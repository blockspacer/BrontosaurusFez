using System;
using System.IO;
using System.Xml.Serialization;
using MoonSharp.Interpreter;
using MoonSharp.Interpreter.Loaders;
using UnityEditor;
using UnityEngine;

namespace Assets.Editor.ExportToLua
{
    public class Utility
    {
        [MenuItem("File/LuaExporter/Set Project Path")]
        static void SetProjectPath()
        {
            ExportSettings exportSettings;
            try
            {
                FileStream file = File.Open(Path.Combine(Application.dataPath, "exportSettings.xml"), FileMode.Open);
                XmlSerializer serializer = new XmlSerializer(typeof(ExportSettings));
                exportSettings = serializer.Deserialize(file) as ExportSettings;
                file.Close();
            }
            catch (FileNotFoundException)
            {
                Debug.Log("! new exportSettings.xml File Will Be Created");
                exportSettings = new ExportSettings();
            }

            exportSettings.ProjectPath = EditorUtility.OpenFolderPanel("Please choose the bin folder of the Project", "",
                "bin");
            if (exportSettings.ProjectPath == String.Empty)
            {
                return;
            }

            FileStream otputStream = File.Open(Path.Combine(Application.dataPath, "exportSettings.xml"), FileMode.Create);
            XmlSerializer outSerializer = new XmlSerializer(typeof(ExportSettings));
            outSerializer.Serialize(otputStream, exportSettings);
            otputStream.Close();
        }

        public static Script CreateANewScript(ExportSettings someExportSettings)
        {
            Script script = new Script();
            script.Options.ScriptLoader = new FileSystemScriptLoader();
            ((ScriptLoaderBase)script.Options.ScriptLoader).ModulePaths = new [] {someExportSettings.ProjectPath + "/Scripts/?", someExportSettings.ProjectPath + "/Scripts/?.lua" };
            script.Options.DebugPrint = Debug.Log;
            return script;
        }

        public static Script CreateAndRunScript(ExportSettings someExportSettings, String aFilePath)
        {
            Script script = Utility.CreateANewScript(someExportSettings);
            DynValue definesValue = DynValue.NewTable(script);
            definesValue.Table["Unity"] = 0;
            script.Globals["globalDefines"] = definesValue;

            try
            {
                try
                {
                    script.DoFile(someExportSettings.ProjectPath + aFilePath);
                }
                catch (ScriptRuntimeException ex)
                {
                
                    Debug.LogError("Lua Error " + ex.DecoratedMessage);
                    throw;
                }
            }
            catch (SyntaxErrorException ex)
            {
                Debug.LogError("Lua Error " + ex.DecoratedMessage);
                throw;
            }

            return script;
        }

        public static Table CreateLuaMatrix(Matrix4x4 aMatrix, Script aScript)
        {
            Table luaMatrix = new Table(aScript);

            luaMatrix["m11"] = aMatrix.transpose.m00;
            luaMatrix["m12"] = aMatrix.transpose.m01;
            luaMatrix["m13"] = aMatrix.transpose.m02;
            luaMatrix["m14"] = aMatrix.transpose.m03;

            luaMatrix["m21"] = aMatrix.transpose.m10;
            luaMatrix["m22"] = aMatrix.transpose.m11;
            luaMatrix["m23"] = aMatrix.transpose.m12;
            luaMatrix["m24"] = aMatrix.transpose.m13;

            luaMatrix["m31"] = aMatrix.transpose.m20;
            luaMatrix["m32"] = aMatrix.transpose.m21;
            luaMatrix["m33"] = aMatrix.transpose.m22;
            luaMatrix["m34"] = aMatrix.transpose.m23;

            luaMatrix["m41"] = aMatrix.transpose.m30;
            luaMatrix["m42"] = aMatrix.transpose.m31;
            luaMatrix["m43"] = aMatrix.transpose.m32;
            luaMatrix["m44"] = aMatrix.transpose.m33;

            return luaMatrix;
        }
    }
}
