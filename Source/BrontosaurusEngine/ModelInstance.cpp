#include "stdafx.h"
#include "ModelInstance.h"
#include "ModelManager.h"
#include "Model.h"
#include "Engine.h"
#include "Renderer.h"


CModelInstance::CModelInstance(const char* aModelPath)
{
	myIsVisible = true;
	myAnimationCounter = 0.f;
	myModel = MODELMGR->LoadModel(aModelPath);
	myHasAnimations = MODELMGR->GetModel(myModel)->HasAnimations();
	myCurrentAnimation = "idle";
}

CModelInstance::CModelInstance(const char* aModelPath, const CU::Matrix44f& aTransformation)
	: CModelInstance(aModelPath)
{
	myTransformation = aTransformation;
}

CModelInstance::CModelInstance(const SShape aShape) 
	: myHasAnimations(false)
{
	myAnimationCounter = 0.f;
	myIsVisible = true;
	myModel = MODELMGR->LoadModel(aShape);
}

CModelInstance::CModelInstance(const SShape aShape, const CU::Matrix44f& aTransformation)
	: CModelInstance(aShape)
{
	myTransformation = aTransformation;
}

CModelInstance::CModelInstance(ModelId aModel, const CU::Matrix44f& aTransformation)
	: myTransformation(aTransformation)
	, myModel(aModel)
	, myAnimationCounter(0.f)
	, myIsVisible(true)
	, myCurrentAnimation("")
	, myHasAnimations(false)
{
}


CModelInstance::~CModelInstance()
{
	if (myModel != NULL_MODEL)
	{
		MODELMGR->GetModel(myModel)->RemoveRef();
		myModel = -1; 
	}
	
	
	//TODO: memoryleek mebe // still exists in ModelManager <-- looks like model manager owns this but a refcount would maybe be something
	//SAFE_DELETE(mySceneAnimator); //is deleted through the map
}

void CModelInstance::Render(Lights::SDirectionalLight* aLight, CU::GrowingArray<CPointLightInstance*>* aPointLightList)
{
	if (ShouldRender() == true)
	{
		SRenderModelMessage* msg = nullptr;
		(myHasAnimations != false) ? msg = new SRenderAnimationModelMessage() : msg = new SRenderModelMessage();

		msg->myDirectionalLight = aLight;
		msg->myPointLights = aPointLightList;
		msg->myModelID = myModel;
		msg->myTransformation =	myTransformation;
		msg->myLastFrameTransformation = myLastFrame;
		
		if (myHasAnimations != false)
		{
			msg->myAnimationTime = myAnimationCounter;
			msg->myCurrentAnimation = myCurrentAnimation;
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
	return  myModel != -1 && myIsVisible == true;
}

//in worldSpace
CU::AABB CModelInstance::GetModelBoundingBox()
{
	CU::AABB box;

	if (!myModel)
		return box;

	box = MODELMGR->GetModel(myModel)->GetBoundingBox();
	box.myCenterPos = box.myCenterPos * myTransformation;
	box.myMinPos = box.myMinPos * myTransformation;
	box.myMaxPos = box.myMaxPos * myTransformation;
	//box.myRadius *= myTransformation.m11;
	
	return box;
}


void CModelInstance::ChangeAnimation(const char* aAnimationKey)
{
	myCurrentAnimation = aAnimationKey;

	/*if (mySceneAnimator != nullptr)
	{
		auto it = mySceneAnimators.find(aAnimationKey);
		if (it != mySceneAnimators.end())
		{
			mySceneAnimator = &it->second;
		}
	}*/
}
