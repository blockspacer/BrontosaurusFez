#include "stdafx.h"
#include "ModelInstance.h"
#include "ModelManager.h"
#include "Model.h"
#include "Engine.h"
#include "Renderer.h"
#include "AnimationController.h"


CModelInstance::CModelInstance(const char* aModelPath)
{
	myIsVisible = true;
	myAnimationCounter = 0.f;
	myModel = MODELMGR->LoadModel(aModelPath);
	
	mySceneAnimator = nullptr;
	if (myModel->GetScene()->HasAnimations())
	{
		mySceneAnimator = new CSceneAnimator();
		mySceneAnimator->Init(myModel->GetScene());
		mySceneAnimator->PlayAnimationForward();
	}
}

CModelInstance::CModelInstance(const char* aModelPath, const CU::Matrix44f& aTransformation)
	: CModelInstance(aModelPath)
{
	myTransformation = aTransformation;
}

CModelInstance::CModelInstance(const SShape aShape)
{
	myAnimationCounter = 0.f;
	myIsVisible = true;
	myModel = MODELMGR->LoadModel(aShape);
	mySceneAnimator = nullptr;
}

CModelInstance::CModelInstance(const SShape aShape, const CU::Matrix44f& aTransformation)
	: CModelInstance(aShape)
{
	myTransformation = aTransformation;
}

CModelInstance::CModelInstance(CModel* aModel, const CU::Matrix44f& aTransformation)
	: myTransformation(aTransformation)
	, myModel(aModel)
	, mySceneAnimator(nullptr)
	, myAnimationCounter(0.f)
	, myIsVisible(true)
{
}

bool CModelInstance::IsAlpha()
{
	return myModel->IsAlphaModel();
}

CModelInstance::~CModelInstance()
{
	myModel = nullptr; //TODO: memoryleek mebe // still exists in ModelManager <-- looks like model manager owns this but a refcount would maybe be something
	SAFE_DELETE(mySceneAnimator);
}

void CModelInstance::Render(Lights::SDirectionalLight* aLight, CU::GrowingArray<CPointLightInstance*>* aPointLightList)
{
	if (myModel != nullptr && myModel->GetInitialized() == true && ShouldRender() == true)
	{
		SRenderModelMessage* msg = nullptr;
		(mySceneAnimator != nullptr) ? msg = new SRenderAnimationModelMessage() : msg = new SRenderModelMessage();

		msg->myDirectionalLight = aLight;
		msg->myPointLights = aPointLightList;
		msg->myModel = myModel;
		msg->myTransformation =	myTransformation;
		msg->myLastFrameTransformation = myLastFrame;
		
		if (mySceneAnimator != nullptr)
		{
			std::vector<mat4>& transforms = mySceneAnimator->GetTransforms(myAnimationCounter);
			SRenderAnimationModelMessage* animMsg = static_cast<SRenderAnimationModelMessage*>(msg);
			memcpy(static_cast<void*>(animMsg->myBoneMatrices), &transforms[0], min(sizeof(animMsg->myBoneMatrices), transforms.size() * sizeof(mat4)));
		}

		RENDERER.AddRenderMessage(msg);

		myLastFrame = myTransformation;
	}
}

void CModelInstance::Update(const CU::Time aDeltaTime)
{
	myAnimationCounter += aDeltaTime.GetSeconds();
}

void CModelInstance::SetTransformation(CU::Matrix44f& aTransformation)
{
	myTransformation = aTransformation;
}

void CModelInstance::SetPosition(CU::Vector3f aPosition)
{
	myTransformation.m41 += aPosition.x;
	myTransformation.m42 += aPosition.y;
	myTransformation.m43 += aPosition.z;

}

bool CModelInstance::ShouldRender()
{
	return myIsVisible == true && myModel != nullptr && myModel->GetInitialized() == true;
}

//in worldSpace
CU::AABB CModelInstance::GetModelBoundingBox()
{
	CU::AABB box;

	if (!myModel)
		return box;

	box = myModel->GetBoundingBox();
	box.myCenterPos = box.myCenterPos * myTransformation;
	box.myMinPos = box.myMinPos * myTransformation;
	box.myMaxPos = box.myMaxPos * myTransformation;
	//box.myRadius *= myTransformation.m11;
	
	return box;
}

void CModelInstance::ChangeAnimation(const char* aAnimationKey)
{
	if (mySceneAnimator != nullptr)
	{
		if (mySceneAnimator->SetAnimation(aAnimationKey) == true)
		{
			mySceneAnimator->PlayAnimationForward();
			myAnimationCounter = 0.f;
		}
	}
}
