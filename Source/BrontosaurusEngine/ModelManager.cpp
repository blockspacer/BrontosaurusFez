#include "stdafx.h"
#include "ModelManager.h"
#include "ModelLoader.h"
#include "ConstBufferTemplate.h"
#include <CommonUtilities.h>
#include <CommandLineManager.h>
#include "Engine.h"
//#include "ModelInstance.h"
//#include "Sprite.h"
#include <matrix44.h>
#include "DynamicString.h"
#include <ThreadPool.h>

#include "GUIModleHelper.h"
#include "FBXLoader.h"
#include "..\GUI\GUIPixelConstantBuffer.h"

CModelManager::CModelManager()
	: myModelList(ourMaxNumberOfModels)
	, myGUIModelList(ourMaxNumberOfGUIModels)
{
	myThreadPool = CEngine::GetInstance()->GetThreadPool();
	//myModelList.Init(8);
	myDebugModelList.Init(64);
}

CModelManager::~CModelManager()
{
}

CModel* CModelManager::LoadModel(CU::DynamicString aModelPath)
{
	if (myModelList.Size() >= ourMaxNumberOfModels)
	{
		DL_MESSAGE_BOX("Too many unique models created! Current max is: %i.\nTalk to programmerz if you want more, it is probably possible.\n", ourMaxNumberOfModels);
		DL_ASSERT("Too many unique models created! Current max is: %i.\n", ourMaxNumberOfModels);
		return nullptr;
	}

	if (myModels.count(aModelPath.c_str()) == 0) //check if derp, else flip bool
	{
		const ModelId newModelId = myModelList.Size();
		myModels[aModelPath.c_str()] = newModelId;
		myModelList.Add(CModel());

//#define THREAD_MODELLOADING  

 // trådad funkar inte atm, för att vi hinner returnera och behandla modellen innan kollisionsdatan är klar.
// Kolla hur modellen är hanterad och se om det inte går o fixa coll med lite pekarmagi.
#ifdef THREAD_MODELLOADING
		auto LoadFunc = [aModelPath, newModelId, this]()
		{
			CreateModel(aModelPath, newModelId);
		};
		
		CU::DynamicString logmsg = aModelPath.c_str();
		logmsg += "\t took %f ms to load.";
		
		myThreadPool->AddWork(CU::Work(LoadFunc, DL_Debug::eLogTypes::eThreadedModels, logmsg.c_str(),CU::ePriority::eHigh));
#else
		CreateModel(aModelPath, newModelId);
#endif

	}
	//myModelList[myModels[aModelPath.c_str()]].AddInstance(aModelInstance);
	return &myModelList[myModels[aModelPath.c_str()]];
}

bool CModelManager::DoesModelExists(const char * aModelPath)
{
	return myModels.find(aModelPath) == myModels.end();
}

CModel* CModelManager::LoadModel(SShape aModelShape)
{
	if (aModelShape.shape == eModelShape::eSphere)
	{
		if(myDebugSphereMap.find(aModelShape.size) == myDebugSphereMap.end())
		{
			const ModelId newModelId = myDebugModelList.Size();
			CModel* temp = CreateShape(aModelShape);
			myDebugModelList.Add(*temp);
			delete temp;
			myDebugSphereMap[aModelShape.size] = newModelId;
		}
		return &myDebugModelList[myDebugSphereMap[aModelShape.size]];
	}
	else if (DoesShapeExist(aModelShape) == false)
	{
		myModelList.Add(*CreateShape(aModelShape));
		myShapes[aModelShape.shape] = myModelList.Size() - 1;
	}

	return &myModelList[myShapes[aModelShape.shape]];
}

bool CModelManager::DoesShapeExist(SShape aModelShape)
{
	if (myShapes.find(aModelShape.shape) == myShapes.end())
	{
		return false;
	}
	return true;
}

CModel* CModelManager::LoadGUIModel(const CLoaderMesh* aLoaderMesh, const char* aTexturePath)
{
	if (myGUIModelList.Size() >= ourMaxNumberOfGUIModels)
	{
		DL_MESSAGE_BOX("Too many unique gui models created! Current max is: %i.\nTalk to programmerz if you want more, it is probably possible.\n", ourMaxNumberOfGUIModels);
		DL_ASSERT("Too many unique models created! Current max is: %i.\n", ourMaxNumberOfGUIModels);
		return nullptr;
	}

	if (myGUIModels.find(aLoaderMesh->myName) == myGUIModels.end())
	{
		CEffect* effect = GUIModleHelper::CreateEffect(aLoaderMesh);
		CSurface* surface = GUIModleHelper::CreateSurface(aTexturePath);

		ModelId handle = myGUIModelList.Size();
		myGUIModels[aLoaderMesh->myName] = handle;

		myGUIModelList.Add(CModel());
		myGUIModelList.GetLast().Initialize(effect, surface, aLoaderMesh);

		SPixelConstantBuffer bufferStruct = {};
		ID3D11Buffer* pixelConstantBuffer = BSR::CreateCBuffer<SPixelConstantBuffer>(&bufferStruct);
		myGUIModelList.GetLast().AddConstantBuffer(CModel::eShaderStage::ePixel, pixelConstantBuffer);
	}

	return &myGUIModelList[myGUIModels[aLoaderMesh->myName]];
}

CModel* CModelManager::LoadGUIModel(const CLoaderMesh * aLoaderMesh, const CU::GrowingArray<CU::DynamicString>& aTexturePaths)
{
	if (myGUIModelList.Size() >= ourMaxNumberOfGUIModels)
	{
		DL_MESSAGE_BOX("Too many unique gui models created! Current max is: %i.\nTalk to programmerz if you want more, it is probably possible.\n", ourMaxNumberOfGUIModels);
		DL_ASSERT("Too many unique models created! Current max is: %i.\n", ourMaxNumberOfGUIModels);
		return nullptr;
	}

	if (myGUIModels.find(aLoaderMesh->myName) == myGUIModels.end())
	{
		CEffect* effect = GUIModleHelper::CreateEffect(aLoaderMesh);
		CSurface* surface = GUIModleHelper::CreateSurface(aTexturePaths);

		ModelId handle = myGUIModelList.Size();
		myGUIModels[aLoaderMesh->myName] = handle;

		myGUIModelList.Add(CModel());
		myGUIModelList.GetLast().Initialize(effect, surface, aLoaderMesh);

		SPixelConstantBuffer bufferStruct = {};
		ID3D11Buffer* pixelConstantBuffer = BSR::CreateCBuffer<SPixelConstantBuffer>(&bufferStruct);
		myGUIModelList.GetLast().AddConstantBuffer(CModel::eShaderStage::ePixel, pixelConstantBuffer);
	}

	return &myGUIModelList[myGUIModels[aLoaderMesh->myName]];
}


void CModelManager::CreateModel(const CU::DynamicString& aModelPath, ModelId aNewModel)
{
	//myModelList[aNewModel] = CModel();
	//CModel * const tempModelPointer = new CModel;
	CModelLoader::LoadModel(aModelPath.c_str(), &myModelList[aNewModel]/*tempModelPointer*/);
	//myModelList[aNewModel] = *tempModelPointer;
}

CModel* CModelManager::CreateShape(SShape aModelShape)
{
	return CModelLoader::CreateShape(aModelShape);
}
