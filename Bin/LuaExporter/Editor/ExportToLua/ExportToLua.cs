using System;
using System.IO;
using Assets.Editor.ExportToLua;
using UnityEngine;
using UnityEditor;
using MoonSharp.Interpreter;
using UnityEngine.SceneManagement;

public class ExportToLua : EditorWindow
{
    [MenuItem("File/LuaExporter/Export")]
    public static void ExportToLuaScript()
    {
        ExportSettings exportSettings = ExportSettings.LoadExportSettings();

        string levelsFolder = exportSettings.ProjectPath + "/Scripts/Levels/";
        string stringStringThatToFileGo;
        string sceneName = SceneManager.GetActiveScene().name;

        {
            Script script = Utility.CreateAndRunScript(exportSettings, "/Scripts/main.lua");

            DynValue levelsTable = script.Globals.Get("GameLevels");

            Table resultTable = new Table(script);

            if (levelsTable.Type == DataType.Table)
            {
                foreach (DynValue tableKey in levelsTable.Table.Keys)
                {
                    if (tableKey.Type == DataType.String)
                    {
                        resultTable[tableKey.String] = tableKey.String;
                    }
                }
            }

            resultTable[sceneName] = "require(\"Levels." + sceneName + "\")";

            DynValue[] arguments = new DynValue[3];

            arguments[0] = DynValue.NewTable(resultTable);
            arguments[1] = DynValue.NewString("GameLevels");
            arguments[2] = DynValue.NewBoolean(false);

            DynValue luaSerializeFunc = script.Globals.Get("GlobalUtilities").Table.Get("SeriaLizeTable");

            if (luaSerializeFunc.Type != DataType.Function)
            {
                Debug.LogError("Serialize table function missing or faulty");
                throw new Exception();
            }
            DynValue results;

            Debug.Log("first call");
            try
            {
                results = luaSerializeFunc.Function.Call(arguments);
            }
            catch (SyntaxErrorException e)
            {
                Debug.LogError(e.DecoratedMessage);
                throw;
            }
        

            File.WriteAllText(levelsFolder + "Levels.lua", results.String);

            int currentIndex = 0;

            const string tableName = "level.myObjects";
            string outString = tableName + " = {}\n\n";

            foreach (string objectType in exportSettings.ObjectTypes)
            {
                GameObject[] gameObjects = GameObject.FindGameObjectsWithTag("BK:" + objectType);

                foreach (GameObject gameObject in gameObjects)
                {
                    string tempTableIndex = tableName + "[" + (currentIndex + 1) + "]";
                    outString += tempTableIndex + " = GlobalUtilities.Copy(GameObjects."+ objectType +")\n";
                    Table luaMatrix = Utility.CreateLuaMatrix(gameObject.transform.localToWorldMatrix, script);

                    arguments[0] = DynValue.NewTable(luaMatrix);
                    arguments[1] = DynValue.NewString(tempTableIndex + ".transformation");
                    arguments[2] = DynValue.NewBoolean(true);

                    Debug.Log("call" + (currentIndex + 1));
                    try
                    {
                        outString += luaSerializeFunc.Function.Call(arguments).String;
                    }
                    catch (SyntaxErrorException e)
                    {
                        Debug.LogError(e.DecoratedMessage);
                        throw;
                    }

                    outString +="\n"+ tempTableIndex + ".myName = \"" + gameObject.name + "\"";
                    outString += "\n";
                    ++currentIndex;
                }
            }

            Table substitutionTable = new Table(script);

            substitutionTable["__objectTable__"] = outString;
            substitutionTable["__levelName__"] = sceneName;

            DynValue generateCodeFromTemplateFunc = script.Globals.Get("GlobalUtilities").Table.Get("GenerateCodeFromTemplate");

            if (luaSerializeFunc.Type != DataType.Function)
            {
                Debug.LogError(" GenerateCodeFromTemplate function missing or faulty");
                throw new Exception();
            }

            DynValue[] args = new DynValue[2];
            string tthtththththt = File.ReadAllText(exportSettings.ProjectPath + "/Scripts/Templates/Level.luat");
            args[0] = DynValue.NewString(tthtththththt);
            args[1] = DynValue.NewTable(substitutionTable);
            DynValue stringThatToFileGo;

            try
            {
                try
                {
                    stringThatToFileGo = generateCodeFromTemplateFunc.Function.Call(args);
                }
                catch (ScriptRuntimeException e)
                {
                    Debug.LogError(e.DecoratedMessage);
                    throw;
                }
            }
            catch (SyntaxErrorException e)
            {
                Debug.LogError(e.DecoratedMessage);
                throw;
            }

            stringStringThatToFileGo = stringThatToFileGo.String;
        }
        

        File.WriteAllText(levelsFolder + "/" + sceneName + ".lua", stringStringThatToFileGo);
    }
}