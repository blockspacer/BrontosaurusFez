#include "stdafx.h"
#include "ModelInstance.h"
#include "ModelManager.h"
#include "Model.h"
#include "Engine.h"
#include "Renderer.h"


CModelInstance::CModelInstance(const char* aModelPath)
{
	myIsVisible = true;
	myModel = MODELMGR->LoadModel(aModelPath);
	myLastFrame = myTransformation;
}

CModelInstance::CModelInstance(SShape aShape)
{
	myIsVisible = true;
	myModel = MODELMGR->LoadModel(aShape);
}


CModelInstance::CModelInstance(const char* aModelPath, CU::Matrix44f& aTransformation)
{
	myIsVisible = true;
	myModel = MODELMGR->LoadModel(aModelPath);
	myTransformation = aTransformation;
}

CModelInstance::CModelInstance(SShape aShape, CU::Matrix44f& aTransformation)
{
	myIsVisible = true;
	myModel = MODELMGR->LoadModel(aShape);
	myTransformation = aTransformation;
}

CModelInstance::CModelInstance(CModel* aModel, const CU::Matrix44f& aTransformation)
	: myTransformation(aTransformation)
	, myModel(aModel)
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
}

void CModelInstance::Render(Lights::SDirectionalLight* aLight, CU::GrowingArray<CPointLightInstance*>* aPointLightList)
{
	if (myModel != nullptr && myModel->GetInitialized() == true && ShouldRender() == true)
	{
		//myModel->Render(myTransformation, myLastFrame, aLight, aPointLightList);

		SRenderModelMessage msg;
		msg.myDirectionalLight = aLight;
		msg.myPointLights = aPointLightList;
		msg.myModel = myModel;
		msg.myTransformation =	myTransformation;
		msg.myLastFrameTransformation = myLastFrame;
		RENDERER.AddRenderMessage(new SRenderModelMessage(msg));


		myLastFrame = myTransformation;


	}
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