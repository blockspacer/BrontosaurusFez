#include "stdafx.h"
#include "Scene.h"
#include "ModelInstance.h"
#include "Engine.h"
#include "ModelManager.h"
#include "PointLightInstance.h"
#include "Camera.h"
#include "Renderer.h"
#include "Skybox.h"
#include "ParticleEmitterInstance.h"


namespace 
{
	float Distance2(const CU::Vector3f& p1, const CU::Vector3f& p2)
	{
		return ((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z));
	}

	int partition(CU::GrowingArray<CModelInstance*, InstanceID>& A, int p, int q);
	void quickSort(CU::GrowingArray<CModelInstance*, InstanceID>& A, int p, int q)
	{
		int r;
		if (p < q)
		{
			r = partition(A, p, q);
			quickSort(A, p, r);
			quickSort(A, r + 1, q);
		}
	}


	int partition(CU::GrowingArray<CModelInstance*, InstanceID>& A, int p, int q)
	{
		float x = Distance2(A[p]->GetTransformation().GetPosition(), CAMERA->GetPosition());
		int i = p;
		int j;

		CModelInstance* temp = nullptr;

		for (j = p + 1; j < q; j++)
		{
			float y = Distance2(A[j]->GetTransformation().GetPosition(), CAMERA->GetPosition());
			if (y > x)
			{
				i = i + 1;
				temp = A[i];
				A[i] = A[j];
				A[j] = temp;
			}

		}

		temp = A[i];
		A[i] = A[p];
		A[p] = temp;

		return i;
	}

}



CScene::CScene()
{
	myActiveCamera = nullptr;
	myModels.Init(64);
	myAlphaModels.Init(8);
	myPointLights.Init(8);
	myParticleEmitters.Init(8);
	mySkybox = nullptr;
}

CScene::~CScene()
{
	SAFE_DELETE(mySkybox);
	myModels.DeleteAll();
	myAlphaModels.DeleteAll();
	myParticleEmitters.DeleteAll();
	//myDebugObjects.DeleteAll();
}

void CScene::Update()
{

}

void CScene::Render()
{
	SSetCameraMessage cameraMsg;
	cameraMsg.myCamera = *CAMERA;
	RENDERER.AddRenderMessage(new SSetCameraMessage(cameraMsg));


	SChangeStatesMessage statemsg;
	if (mySkybox != nullptr)
	{
		statemsg.myBlendState = eBlendState::eNoBlend;
		statemsg.myRasterizerState = eRasterizerState::eNoCulling;
		statemsg.myDepthStencilState = eDepthStencilState::eDisableDepth;
		statemsg.mySamplerState = eSamplerState::eWrap;

		RENDERER.AddRenderMessage(new SChangeStatesMessage(statemsg));

		SRenderSkyboxMessage msg;
		msg.mySkybox = mySkybox;
		RENDERER.AddRenderMessage(new SRenderSkyboxMessage(msg));

		statemsg.myRasterizerState = eRasterizerState::eDefault;
		statemsg.myDepthStencilState = eDepthStencilState::eDefault;
		statemsg.myBlendState = eBlendState::eNoBlend;
		statemsg.mySamplerState = eSamplerState::eClamp;

		RENDERER.AddRenderMessage(new SChangeStatesMessage(statemsg));
	}

	for (unsigned int i = 0; i < myModels.Size();++i)
	{
		if (myModels[i] == nullptr || myModels[i]->ShouldRender() == false)
		{
			continue;
		}

		if (CAMERA->IsInside(myModels[i]->GetModelBoundingBox()) == false)
		{
			continue;
		}



		//SRenderModelMessage msg;
		//msg.myDirectionalLight = &myDirectionalLight;
		//msg.myPointLights = &myPointLights;
		//msg.myModel = myModels[i]->GetModel();
		//msg.myTransformation = myModels[i]->GetTransformation();
		//msg.myLastFrameTransformation = myModels[i]->GetLastFrameTransformation();
		//RENDERER.AddRenderMessage(new SRenderModelMessage(msg));



		myModels[i]->Render(&myDirectionalLight, &myPointLights);

	}


	statemsg.myBlendState = eBlendState::eAlphaBlend;
	statemsg.myRasterizerState = eRasterizerState::eDefault;
	statemsg.myDepthStencilState = eDepthStencilState::eDefault;
	statemsg.mySamplerState = eSamplerState::eWrap;


	RENDERER.AddRenderMessage(new SChangeStatesMessage(statemsg));


	quickSort(myAlphaModels, 0, myAlphaModels.Size());


	for (unsigned int i = 0; i < myAlphaModels.Size(); ++i)
	{
		if (myAlphaModels[i] == nullptr || myAlphaModels[i]->ShouldRender() == false)
		{
			continue;
		}

		if (CAMERA->IsInside(myAlphaModels[i]->GetModelBoundingBox()) == false)
		{
			continue;
		}

		myAlphaModels[i]->Render(&myDirectionalLight, &myPointLights);

	}

	statemsg.myBlendState = eBlendState::eAlphaBlend;
	statemsg.myRasterizerState = eRasterizerState::eDefault;
	statemsg.myDepthStencilState = eDepthStencilState::eDisableDepth;
	statemsg.mySamplerState = eSamplerState::eClamp;

	RENDERER.AddRenderMessage(new SChangeStatesMessage(statemsg));

	for (unsigned int i = 0; i < myParticleEmitters.Size(); ++i)
	{
		/*if (myParticleEmitters[i]->IsVisible() == false)
		{
			continue;
		}*/

		myParticleEmitters[i]->Render();
	}
}

InstanceID CScene::AddModelInstance(CModelInstance* aModelInstance)
{
	InstanceID id = 0;

	if (aModelInstance->IsAlpha() == false)
	{
		if (myFreeModels.Size() < 1)
		{
			myModels.Add(aModelInstance);
			id = myModels.Size();
			return id;
		}

		id = myFreeModels.Pop();
		myModels[id] = aModelInstance;
	}
	else
	{
		if (myFreeAlphaModels.Size() < 1)
		{
			myAlphaModels.Add(aModelInstance);
			id = myAlphaModels.Size();
			return id;
		}

		id = myFreeAlphaModels.Pop();
		myAlphaModels[id] = aModelInstance;
	}
	
	return id;
}

InstanceID CScene::AddDirectionalLight(Lights::SDirectionalLight & aDirectionalLight)
{
	myDirectionalLight = aDirectionalLight;
	return 0;
}

InstanceID CScene::AddPointLightInstance(CPointLightInstance* aPointLight)
{
	myPointLights.Add(aPointLight);
	return 0;
}

InstanceID CScene::AddParticleEmitterInstance(CParticleEmitterInstance * aParticleEmitterInstance)
{
	if (myFreeParticleEmitters.Size() < 1)
	{
		InstanceID temp = myParticleEmitters.Size();
		myParticleEmitters.Add(aParticleEmitterInstance);
		return temp;
	}

	const InstanceID tempId = myFreeParticleEmitters.Pop();
	myParticleEmitters[tempId] = aParticleEmitterInstance;

	return  tempId;
}

InstanceID CScene::AddCamera(CU::Camera & aCamera)
{
	myCameras.Add(aCamera);
	return 0;
}

void CScene::SetCamera(CU::Camera * aCamera)
{
	myActiveCamera = aCamera;
}

void CScene::SetSkybox(const char* aPath)
{
	if (mySkybox != nullptr)
	{
		delete mySkybox;
	}

	mySkybox = new CSkybox();
	mySkybox->Init(aPath);
}

CModelInstance& CScene::GetModelAt(InstanceID aModelID)
{
	return *myModels[aModelID];
}

void CScene::DeleteModelInstance(CModelInstance* anInstance)
{
	InstanceID currentId;

	if (myModels.Find(anInstance, currentId) == false)
	{
		return;
	}

	DeleteModelInstance(currentId);
}

void CScene::DeleteModelInstance(const InstanceID& anId)
{
	//if thread lock here
	CModelInstance* currentModel = myModels[anId];
	myModels[anId] = nullptr;
	myFreeModels.Push(anId);
	//if thread unlock here

	delete currentModel;
}

void CScene::DeleteParticleEmitterInstance(const InstanceID anID)
{
	//if thread lock here
	CParticleEmitterInstance* currentEmitter = myParticleEmitters[anID];
	myParticleEmitters[anID] = nullptr;
	myFreeParticleEmitters.Push(anID);
	//if thread unlock here

	// Deletas i emitterInstance tror jag?
	//delete currentEmitter;
}