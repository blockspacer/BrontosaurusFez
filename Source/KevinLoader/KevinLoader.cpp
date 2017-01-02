#include "stdafx.h"
#include "KevinLoader.h"
#include "../CommonUtilities/DL_Debug.h"
#include "../CommonUtilities/PJWrapper.h"
#include "../BrontosaurusEngine/Console.h"
#include <fstream>

namespace KLoader
{
	CKevinLoader* CKevinLoader::ourInstance = nullptr;

	LinkObject::LinkObject(): myIndex(0)
	{
		myChildren.Init(1);
	}

	void CKevinLoader::CreateInstance()
	{
		if (ourInstance == nullptr)
		{
			ourInstance = new CKevinLoader;
		}
	}

	CKevinLoader& CKevinLoader::GetInstance()
	{
		if (ourInstance == nullptr)
		{
			DL_ASSERT("CKevinLoader(JsonImporter) has not been created");
		}

		return *ourInstance;
	}

	void CKevinLoader::DestroyInstance()
	{
		if (ourInstance != nullptr)
		{
			delete ourInstance;
			ourInstance = nullptr;
		}
	}

	void CKevinLoader::RegisterComponentLoadFunction(const std::string& ComponentType, const LoadFunction& aFunction)
	{
		myLoadFunctions[ComponentType] = aFunction;
	}

	void CKevinLoader::RegisterComponentLinkFunction(const std::string& ComponentType, const LinkFunction& aFunction)
	{
		myLinkFunctions[ComponentType] = aFunction;
	}

	void CKevinLoader::RegisterObjectLoadFunction(const LoadFunction& aFunction)
	{
		myObjectLoadFunction = aFunction;
	}

	void CKevinLoader::RegisterObjectLinkFunction(const LinkFunction& aLinkFunction)
	{
		myObjectLinkFunction = aLinkFunction;
	}

	eError CKevinLoader::LoadFile(std::wstring aFilePath)
	{
		CU::CPJWrapper wrapper;
		wrapper.Parse(std::string(aFilePath.begin(), aFilePath.end()));

		const eError loadObjectError = LoadObjects(wrapper);
		if (loadObjectError != eError::NO_LOADER_ERROR)
		{
			return loadObjectError;
		}

	}

	void CKevinLoader::PrintMissingComponents(CU::DynamicString componentName)
	{
#ifndef _RETAIL_BUILD
		const char * fileName = "IMPORTANT_FOR_PROG_MISSING_COMPONENTS.txt";

		std::ofstream missingComponetsFile;
		if (myHasLoadedFile == false)
		{
			missingComponetsFile.open(fileName, std::ofstream::trunc);
			missingComponetsFile << "Components That was not Loaded --------------------------------------------------" << std::endl;
			myHasLoadedFile = true;
		}
		else
		{
			missingComponetsFile.open(fileName, std::ofstream::app);
		}

		const CU::DynamicString outString = componentName + " ---> was not loded due to note beeing suported yet!!   poke a prog to fix or if prog, fix plz";

		DL_PRINT(outString.c_str());
		missingComponetsFile << outString.c_str() << std::endl;
		missingComponetsFile.close();
#endif
	}

	CKevinLoader::CKevinLoader()
	{
#ifndef _RETAIL_BUILD
		myHasLoadedFile = false;
#endif
	}

	CKevinLoader::~CKevinLoader()
	{
	}

	int CKevinLoader::LoadComponent(const std::string& aIdString, const CU::CPJWrapper& aRoot)
	{
		const CU::JsonObject rootObject = aRoot.GetJsonObject();
		const CU::JsonObject componentsObject = rootObject.at("components").GetJsonObject();

		if (componentsObject.count(aIdString) == 0)
		{
			return 0;
		}

		const CU::JsonObject currentComponent = componentsObject.at(aIdString).GetJsonObject();
		const std::string type = currentComponent.at("type").GetString();
		if (myLoadFunctions.count(type) == 0)
		{
			PrintMissingComponents(type.c_str());
			return 0;
		}

		SLoadedComponentData componentData;

		componentData.myTypeName = type;
		componentData.myData = currentComponent.at("data").GetJsonObject();

		return myLoadFunctions[type](componentData);
	}

	eError CKevinLoader::LoadObjects(const CU::CPJWrapper& aRoot)
	{
		CU::JsonObject rootObject = aRoot.GetJsonObject();

		if (rootObject.count("objects") == 0 || rootObject["objects"].IsArray() == false )
		{
			myError = "LevelData json document missing objects array";
			return eError::MISSING_OBJECTS;
		}

		const CU::CPJWrapper objectsArray = rootObject["objects"];

		for (int i = 0; i < objectsArray.Size(); ++i)
		{
			const CU::CPJWrapper currentObject= objectsArray[i];
			SLoadedComponentData componentData;
			CU::JsonObject data;

			componentData.myTypeName = currentObject.GetJsonObject()["name"].GetString();

			data["name"] = currentObject.GetJsonObject()["name"];
			data["position"] = currentObject.GetJsonObject()["position"];
			data["rotation"] = currentObject.GetJsonObject()["rotation"];
			data["scale"] = currentObject.GetJsonObject()["scale"];

			componentData.myData = data;

			const KId currentID = currentObject.GetJsonObject()["id"].GetNumber() * -1;

			LinkObject currentLink;

			if (myLinkObjects.count(currentID) > 0)
			{
				currentLink = myLinkObjects[currentID];
			}
			
			if (currentObject.GetJsonObject().count("parent") > 0)
			{
				const CU::CPJWrapper parentIDObj = currentObject.GetJsonObject()["parent"];
				const KId parentId = parentIDObj.GetNumber() * -1;
				if (myLinkObjects.count(parentId) == 0)
				{
					myLinkObjects[parentId] = LinkObject();
				}

				myLinkObjects[parentId].myChildren.Add(currentID);
			}

			if (!myObjectLoadFunction)
			{
				return eError::MISSING_OBJECT_LOAD_FUNC;
			}

			 currentLink.myIndex = myObjectLoadFunction(componentData);

			 const CU::CPJWrapper componentsArray = currentObject.GetJsonObject()["components"];
			 for (int j = 0; j < componentsArray.Size(); ++j)
			 {
				 std::string IdString = componentsArray[j].GetString();

				 const int componentId = LoadComponent(IdString, aRoot);

				 if (componentId != 0)
				 {
					 myObjectLinkFunction(currentLink.myIndex, componentId);
				 }
			 }

			 myLinkObjects[currentID] = currentLink;
		}
		return eError::NO_LOADER_ERROR;
	}
}
