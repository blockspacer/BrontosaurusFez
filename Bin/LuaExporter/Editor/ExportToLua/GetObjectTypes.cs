using System;
using System.Collections.Generic;
using System.Linq;
using Assets.Editor.ExportToLua;
using MoonSharp.Interpreter;
using MoonSharp.Interpreter.Loaders;
using UnityEngine;
using UnityEditor;

public class ObjectTypeLoader
{
    public static void AddObjectTags(string tag)
    {
        UnityEngine.Object[] asset = AssetDatabase.LoadAllAssetsAtPath("ProjectSettings/TagManager.asset");
        if ((asset != null) && (asset.Length > 0))
        {
            SerializedObject so = new SerializedObject(asset[0]);
            SerializedProperty tags = so.FindProperty("tags");

            for (int i = 0; i < tags.arraySize; ++i)
            {
                if (tags.GetArrayElementAtIndex(i).stringValue == tag)
                {
                    return; // Tag already present, nothing to do.
                }
            }

            tags.InsertArrayElementAtIndex(tags.arraySize);
            tags.GetArrayElementAtIndex(tags.arraySize -1).stringValue = tag;
            so.ApplyModifiedProperties();
            so.Update();
        }
    }

    [MenuItem("File/LuaExporter/Load Object Types")]
    public static void LoadObjectTypes()
    {
        ExportSettings exportSettings = ExportSettings.LoadExportSettings();

        Script script = Utility.CreateANewScript(exportSettings);
        DynValue definesValue = DynValue.NewTable(script);
        definesValue.Table["Unity"] = 0;
        script.Globals["globalDefines"] = definesValue;

        try
        {
            script.DoFile(exportSettings.ProjectPath + "/Scripts/main.lua");
        }
        catch (ScriptRuntimeException ex)
        {
            Debug.LogError("Lua Error" + ex.DecoratedMessage);
            throw;
        }
        


        DynValue gameObjectsValue = script.Globals.Get("GameObjects");

        var objectTypes = gameObjectsValue.Table.Keys.ToList();

        exportSettings.ObjectTypes = new List<string>();

        foreach (DynValue objectType in objectTypes)
        {
            exportSettings.ObjectTypes.Add(objectType.String);
            AddObjectTags("BK:" + objectType.String);
        }

        ExportSettings.SaveExportSettings(exportSettings);
    }
}