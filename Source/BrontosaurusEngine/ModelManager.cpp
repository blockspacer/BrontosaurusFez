#include "stdafx.h"
#include "Engine.h"
#include "ModelManager.h"
#include "ModelLoader.h"
#include "ConstBufferTemplate.h"
#include <CommonUtilities.h>
#include <CommandLineManager.h>
#include <matrix44.h>
#include <StringHelper.h>
#include <ThreadPool.h>
#include <DynamicString.h>

#include "GUIModleHelper.h"
#include "FBXLoader.h"
#include "..\GUI\GUIPixelConstantBuffer.h"


CModelManager::CModelManager()
	: myModelList(ourMaxNumberOfModels)
{
	myThreadPool = CEngine::GetInstance()->GetThreadPool();
}

CModelManager::~CModelManager()
{
}


const CModelManager::ModelId CModelManager::LoadModel(const CU::DynamicString& aModelPath)
{
	if (myModelList.Size() >= ourMaxNumberOfModels)
	{
		DL_MESSAGE_BOX("Too many unique models created! Current max is: %i.\nTalk to a programmer if we need more, it is probably possible.\n", ourMaxNumberOfModels);
		DL_ASSERT("Too many unique models created! Current max is: %i.\n", ourMaxNumberOfModels);
		return NULL_MODEL;
	}

	ModelId newModelID = -1;

	if (myModels.count(aModelPath.c_str()) == 0) //check if derp, else flip bool
	{
		if (myFreeModelIDs.Size() != 0)
		{
			newModelID = myFreeModelIDs.Pop();
		}
		else
		{
			newModelID = myModelList.Size();
			myModelList.Add(CModel());
		}

		myModels[aModelPath.c_str()] = newModelID;


		if (CreateModel(aModelPath, newModelID) == false)
		{
			myModelList.Pop();
			myModels.erase(aModelPath.c_str());
			return NULL_MODEL;
		}

		LoadAnimations(aModelPath.c_str(), newModelID);

	}

	myModelList[myModels[aModelPath.c_str()]].AddRef();
	return myModels[aModelPath.c_str()];
}
const CModelManager::ModelId CModelManager::LoadModel(SShape aModelShape)
{
	if (myModelList.Size() >= ourMaxNumberOfModels)
	{
		DL_MESSAGE_BOX("Too many unique models created! Current max is: %i.\nTalk to a programmer if we need more, it is probably possible.\n", ourMaxNumberOfModels);
		DL_ASSERT("Too many unique models created! Current max is: %i.\n", ourMaxNumberOfModels);
		return -1;
	}

	ModelId newModelID = -1;


	if (aModelShape.shape == eModelShape::eSphere)
	{
		if(mySpheres.find(aModelShape.size) == mySpheres.end())
		{
			if (myFreeModelIDs.Size() != 0)
			{
				newModelID = myFreeModelIDs.Pop();
			}
			else
			{
				newModelID = myModelList.Size();
				myModelList.Add(CModel());
			}

			myModelList[newModelID] = *CreateShape(aModelShape);
			mySpheres[aModelShape.size] = newModelID;
		}
		return mySpheres[aModelShape.size];
	}
	else if (DoesShapeExist(aModelShape) == false)
	{
		if (myFreeModelIDs.Size() != 0)
		{
			newModelID = myFreeModelIDs.Pop();
		}
		else
		{
			newModelID = myModelList.Size();
			myModelList.Add(CModel());
		}

		myModelList[newModelID] = *CreateShape(aModelShape);
		myShapes[aModelShape.shape] = newModelID;
	}

	return myShapes[aModelShape.shape];
}

const CModelManager::ModelId CModelManager::LoadGUIModel(const CLoaderMesh* aLoaderMesh, const char* aTexturePath)
{
	if (myModelList.Size() >= ourMaxNumberOfModels)
	{
		DL_MESSAGE_BOX("Too many unique models created! Current max is: %i.\nTalk to a programmer if you want more, it is probably possible.\n", ourMaxNumberOfModels);
		DL_ASSERT("Too many unique models created! Current max is: %i.\n", ourMaxNumberOfModels);
		return -1;
	}

	if (myModels.find(aLoaderMesh->myName) == myModels.end())
	{
		CEffect* effect = GUIModleHelper::CreateEffect(aLoaderMesh);
		CSurface* surface = GUIModleHelper::CreateSurface(aTexturePath);

		ModelId handle = -1;
		if (myFreeModelIDs.Size() != 0)
		{
			handle = myFreeModelIDs.Pop();
		}
		else
		{
			handle = myModelList.Size();
			myModelList.Add(CModel());
		}


		myModels[aLoaderMesh->myName] = handle;

		myModelList[handle].Initialize(effect, surface, aLoaderMesh);

		SPixelConstantBuffer bufferStruct = {};
		ID3D11Buffer* pixelConstantBuffer = BSR::CreateCBuffer<SPixelConstantBuffer>(&bufferStruct);
		myModelList[handle].AddConstantBuffer(CModel::eShaderStage::ePixel, pixelConstantBuffer);
	}

	return myModels[aLoaderMesh->myName];
}
const CModelManager::ModelId CModelManager::LoadGUIModel(const CLoaderMesh * aLoaderMesh, const CU::GrowingArray<CU::DynamicString>& aTexturePaths)
{
	if (myModelList.Size() >= ourMaxNumberOfModels)
	{
		DL_MESSAGE_BOX("Too many unique models created! Current max is: %i.\nTalk to a programmer if you want more, it is probably possible.\n", ourMaxNumberOfModels);
		DL_ASSERT("Too many unique models created! Current max is: %i.\n", ourMaxNumberOfModels);
		return -1;
	}

	if (myModels.find(aLoaderMesh->myName) == myModels.end())
	{
		CEffect* effect = GUIModleHelper::CreateEffect(aLoaderMesh);
		CSurface* surface = GUIModleHelper::CreateSurface(aTexturePaths);

		ModelId handle = -1;
		if (myFreeModelIDs.Size() != 0)
		{
			handle = myFreeModelIDs.Pop();
		}
		else
		{
			handle = myModelList.Size();
			myModelList.Add(CModel());
		}

		myModels[aLoaderMesh->myName] = handle;

		myModelList[handle].Initialize(effect, surface, aLoaderMesh);

		SPixelConstantBuffer bufferStruct = {};
		ID3D11Buffer* pixelConstantBuffer = BSR::CreateCBuffer<SPixelConstantBuffer>(&bufferStruct);
		myModelList[handle].AddConstantBuffer(CModel::eShaderStage::ePixel, pixelConstantBuffer);
	}

	return myModels[aLoaderMesh->myName];
}

CModel* CModelManager::GetModel(const ModelId aModelID)
{
	if (aModelID == -1 || myModelList[aModelID].GetInitialized() == false)
	{
		return nullptr;
	}
	return &myModelList[aModelID];
}
void CModelManager::RemoveModel(const ModelId aModelID)
{
	myModelList[aModelID].RemoveRef();
	
	if (myModelList[aModelID].GetRefCount() <= 0)
	{
		myFreeModelIDs.Push(aModelID);
		myModelList[aModelID] = CModel();
	}
}
void CModelManager::LoadAnimations(const char * aPath, const ModelId aModelId)
{
	std::string modelName = aPath;
	modelName -= std::string("idle.fbx"); //temporary bc there are no bones in the vertices in player.fbx, only in player_idle.fbx etc
	const ModelId animationCount = 6;
	std::string animationNames[animationCount] = { ("idle"), ("walk"), ("pickup"), ("turnRight90"), ("turnLeft90"), ("attack") };

	CModel* mdl = GetModel(aModelId);
	const aiScene* scene = mdl->GetScene();
	if (mdl != nullptr && scene->HasAnimations())// && mdl->GetScene->HasAnimations())
	{
		bool foundSpecial = false;
		for (int i = 0; i < animationCount; ++i)
		{
			const std::string& animationName = animationNames[i];

			ModelId tempAnimationModel = LoadModel((modelName + animationName + ".fbx").c_str());
			if (tempAnimationModel == NULL_MODEL)
			{
				continue;
			}

			foundSpecial = true;

			mdl->mySceneAnimators[animationName] = CSceneAnimator();
			mdl->mySceneAnimators[animationName].Init(GetModel(tempAnimationModel)->GetScene());

			//RemoveModel(tempAnimationModel);
		}

		if (foundSpecial == false)
		{
			mdl->mySceneAnimators["idle"] = CSceneAnimator();
			mdl->mySceneAnimators["idle"].Init(mdl->GetScene());
		}

		mdl->mySceneAnimator = &mdl->mySceneAnimators["idle"];
		mdl->mySceneAnimator->PlayAnimationForward();
	}
}



bool CModelManager::CreateModel(const CU::DynamicString& aModelPath, ModelId aNewModel)
{
	return CModelLoader::LoadModel(aModelPath.c_str(), &myModelList[aNewModel]/*tempModelPointer*/);
}
CModel* CModelManager::CreateShape(SShape aModelShape)
{
	return CModelLoader::CreateShape(aModelShape);
}



bool CModelManager::DoesModelExists(const char * aModelPath)
{
	return myModels.find(aModelPath) != myModels.end();
}
bool CModelManager::DoesShapeExist(SShape aModelShape)
{
	if (myShapes.find(aModelShape.shape) == myShapes.end())
	{
		return false;
	}
	return true;
}
