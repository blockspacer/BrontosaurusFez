#include "stdafx.h"
#include "ModelInstance.h"
#include "ModelManager.h"
#include "Model.h"
#include "Engine.h"
#include "Renderer.h"
#include "RenderCamera.h"

//Lights
#include "PointLightInstance.h"
#include "Intersection.h"


CModelInstance::CModelInstance(const char* aModelPath)
{
	myIsVisible = true;
	myHighlightIntencity = 0.f;
	myAnimationCounter = 0.f;
	myModel = MODELMGR->LoadModel(aModelPath);
	CModel* model = MODELMGR->GetModel(myModel);
	if (model != nullptr)
	{
		myHasAnimations = model->HasAnimations();
	}
	myCurrentAnimation = "idle";
	myAnimationLooping = true;
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
	, myCurrentAnimation(nullptr)
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

void CModelInstance::Render(Lights::SDirectionalLight* aLight, CU::VectorOnStack<CPointLightInstance, 8>& aPointLightList)
{
	if (ShouldRender() == true)
	{

		SRenderModelMessage* msg = new SRenderModelMessage();
		msg->myModelID = myModel;
		msg->myRenderParams.myTransform = myTransformation;
		msg->myRenderParams.myTransformLastFrame = myLastFrame;
		msg->myRenderParams.aHighlightIntencity = myHighlightIntencity;
		msg->myRenderParams.myRenderToDepth = false;

		msg->myRenderParams.myDirectionalLight = *aLight;
		msg->myRenderParams.myNumLights = 0;
		
		for (unsigned int i = 0; i < aPointLightList.Size(); ++i)
		{
			if (msg->myRenderParams.myPointLightList.SafeAdd(Lights::SPointLight()) == true)
			{
				msg->myRenderParams.myPointLightList.GetLast().color = aPointLightList[i].GetColor();
				msg->myRenderParams.myPointLightList.GetLast().intensity = aPointLightList[i].GetInstensity();
				msg->myRenderParams.myPointLightList.GetLast().position = aPointLightList[i].GetPosition();
				msg->myRenderParams.myPointLightList.GetLast().range = aPointLightList[i].GetRange();
				msg->myRenderParams.myNumLights++;
			}
		}

		
		if (myHasAnimations != false)
		{
			msg->myRenderParams.aAnimationTime = myAnimationCounter;
			msg->myRenderParams.aAnimationState = myCurrentAnimation;
			msg->myRenderParams.aAnimationLooping = myAnimationLooping;
		}

		RENDERER.AddRenderMessage(msg);

		myLastFrame = myTransformation;
	}
}

void CModelInstance::Render(Lights::SDirectionalLight * aLight, CU::VectorOnStack<CPointLightInstance, 8>& aPointLightList, CRenderCamera & aRenderToCamera)
{
	if (ShouldRender() == true)
	{
		SRenderModelMessage* msg = new SRenderModelMessage();
		msg->myModelID = myModel;

		msg->myRenderParams.myTransform = myTransformation;
		msg->myRenderParams.myTransformLastFrame = myLastFrame;
		msg->myRenderParams.aHighlightIntencity = myHighlightIntencity;

		if (aLight == nullptr)
		{
			msg->myRenderParams.myRenderToDepth = true;
			msg->myType = SRenderMessage::eRenderMessageType::eRenderModelDepth;
		}
		else
		{
			CU::Intersection::Sphere model;
			CU::Intersection::Sphere light;
			msg->myRenderParams.myNumLights = 0;


			for (unsigned int i = 0; i < aPointLightList.Size(); ++i)
			{
				if (aPointLightList[i].GetIsActive() == true)
				{
					light.myCenterPosition = aPointLightList[i].GetPosition();
					light.myRadius = aPointLightList[i].GetRange();
					light.myRadiusSquared = light.myRadius * light.myRadius;

					if (CU::Intersection::SphereVsSphere(model, light))
					{
						msg->myRenderParams.myPointLightList.Add(Lights::SPointLight());
						msg->myRenderParams.myPointLightList.GetLast().color = aPointLightList[i].GetColor();
						msg->myRenderParams.myPointLightList.GetLast().intensity = aPointLightList[i].GetInstensity();
						msg->myRenderParams.myPointLightList.GetLast().position = aPointLightList[i].GetPosition();
						msg->myRenderParams.myPointLightList.GetLast().range = aPointLightList[i].GetRange();
						msg->myRenderParams.myNumLights++;
					}
				}
			}
		}
	
		if (myHasAnimations != false)
		{
			msg->myRenderParams.aAnimationTime = myAnimationCounter;
			msg->myRenderParams.aAnimationState = myCurrentAnimation;
			msg->myRenderParams.aAnimationLooping = myAnimationLooping;
		}

		aRenderToCamera.AddRenderMessage(msg);
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
	myTransformation.m41 = aPosition.x;
	myTransformation.m42 = aPosition.y;
	myTransformation.m43 = aPosition.z;
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

void CModelInstance::SetAnimationLooping(const bool aValue)
{
	myAnimationLooping = aValue;
	myAnimationCounter = 0.0f;

}

void CModelInstance::ResetAnimation()
{
	myAnimationCounter = 0.0f;
}

void CModelInstance::SetHighlightIntencity(const float aHighlightIntencity)
{
	myHighlightIntencity = aHighlightIntencity;
}
