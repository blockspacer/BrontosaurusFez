using System.Collections.Generic;
using System.IO;
using System.Xml;
using System.Xml.Serialization;
using UnityEngine;

namespace Assets.Editor.ExportToLua
{
    [XmlRoot("ExportSettings")]
    public class ExportSettings
    { 
        public string ProjectPath { get; set; }
        public List<string> ObjectTypes { get; set; }

        public static ExportSettings LoadExportSettings()
        {
            try
            {
                FileStream inputStream = File.Open(Path.Combine(Application.dataPath, "exportSettings.xml"), FileMode.Open);
                XmlSerializer serializer = new XmlSerializer(typeof(ExportSettings));
                ExportSettings exportSettings = serializer.Deserialize(inputStream) as ExportSettings;
                inputStream.Close();
                return exportSettings;
            }
            catch (FileNotFoundException)
            {
                Debug.LogError("ProjectFilePath not set plaese set it by using File->LuaExporter->Set Project Path in the unity editor");
                throw;
            }
        }

        public static void SaveExportSettings(ExportSettings someExportSettings)
        {
            FileStream otputStream = File.Open(Path.Combine(Application.dataPath, "exportSettings.xml"), FileMode.Create);
            XmlSerializer outSerializer = new XmlSerializer(typeof(ExportSettings));
            outSerializer.Serialize(otputStream, someExportSettings);
            otputStream.Close();
        }
    }
}