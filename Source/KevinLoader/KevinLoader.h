#pragma once
#include <functional>
#include <map>
#include "KLoaderError.h"
#include "../CommonUtilities/PJWrapper.h"
#include "../CommonUtilities/DynamicString.h"
#include "../CommonUtilities/GrowingArray.h"


namespace KLoader
{

	typedef int KId;

	struct LinkObject
	{
		LinkObject();

		int myIndex;

		CU::GrowingArray<KId> myChildren;
	};

	struct SLoadedComponentData
	{
		std::string myTypeName;
		CU::JsonObject myData;
	};

	typedef std::function<int(SLoadedComponentData)> LoadFunction;
	typedef std::function<void(int, int)> LinkFunction;

	class CKevinLoader
	{
	public:
		static void CreateInstance();
		static CKevinLoader& GetInstance();
		static void DestroyInstance();

		void RegisterComponentLoadFunction(const std::string& ComponentType, const LoadFunction& aFunction);
		void RegisterComponentLinkFunction(const std::string& ComponentType, const LinkFunction& aFunction);

		void RegisterObjectLoadFunction(const LoadFunction& aFunction);
		void RegisterObjectLinkFunction(const LinkFunction& aLinkFunction);


		eError LoadFile(std::wstring aFilePath);

		std::string GetError();

		void PrintMissingComponents(CU::DynamicString componentName);

	private:
		CKevinLoader();
		~CKevinLoader();

		int LoadComponent(const std::string& aIdString, const CU::CPJWrapper& aRoot);
		eError LoadObjects(const CU::CPJWrapper& aWrapper);

		std::map<std::string, LoadFunction> myLoadFunctions;
		std::map<std::string, LinkFunction> myLinkFunctions;
		LoadFunction myObjectLoadFunction;
		LinkFunction myObjectLinkFunction;

		std::string myError;

		std::map<KId, LinkObject> myLinkObjects;
		
		static CKevinLoader* ourInstance;
		
#ifndef _RETAIL_BUILD
		bool myHasLoadedFile;
#endif
	};

	inline std::string CKevinLoader::GetError()
	{
		return myError;
	}

}
