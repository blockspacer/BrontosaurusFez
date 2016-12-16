#include "stdafx.h"
#include "ModelComponent.h"
#include "..\BrontosaurusEngine\ModelInstance.h"
#include "GameObject.h"
#include "..\CommonUtilities\matrix44.h"
#include "../BrontosaurusEngine/ModelManager.h"
#include "../BrontosaurusEngine/Engine.h"
#include "ModelComponentManager.h"

CModelComponent::CModelComponent()
{
	myModel = nullptr;
	myType = eComponentType::eModel;
}

CModelComponent::CModelComponent(CModelInstance& aModel)
{
	myModel = &aModel;
	myType = eComponentType::eModel;
}

CModelComponent::CModelComponent(CModelInstance & aModel, const bool aIsDebugSphere)
	: CModelComponent(aModel)
{

}

CModelComponent::~CModelComponent()
{

}

CU::Matrix44f CModelComponent::GetToWorldTransform()
{
	if (GetParent() != nullptr)
	{
		return GetParent()->GetToWorldTransform();
	}
	else
	{
		return CU::Matrix44f::Identity;
	}
}

void CModelComponent::SetVisibility(const bool aVisibility)
{
	myModel->SetVisibility(aVisibility);
}

void CModelComponent::FlipVisibility()
{
	myModel->SetVisibility(!myModel->GetVisibility());
}

void CModelComponent::Receive(const eComponentMessageType aType, const SComponentMessageData &aData)
{
	switch (aType)
	{
	case eComponentMessageType::eAddComponent:
		if (aData.myComponentTypeAdded != eComponentType::eModel) break; //else: fall through
	case eComponentMessageType::eMoving:
		myModel->SetTransformation(GetToWorldTransform());
		break;
	case eComponentMessageType::eStartedMoving:
		ChangeAnimation(aData.myString);
		break;
	case eComponentMessageType::eChangedDirection:
		ChangeDirection(aData.myVector2f);
		break;
	case eComponentMessageType::eStoppedMoving:
		ChangeAnimation(aData.myString);
		break;
	case eComponentMessageType::eSetVisibility:
		SetVisibility(aData.myBool);
		break;
	}
}

void CModelComponent::Destroy()
{
	CModelComponentManager::GetInstance().DeleteModelComponent(this);
	myModel = nullptr;
}

CModelInstance* CModelComponent::GetAndReleaseModel()
{
	CModelInstance* const tempModel = myModel;
	myModel = nullptr;
	return tempModel;
}

void CModelComponent::ChangeAnimation(const char* aAnimationKey)
{
	myModel->ChangeAnimation(aAnimationKey);
}

void CModelComponent::ChangeDirection(const CU::Vector2f& aDirection2D)
{
	//float angle = std::acos(aDirection2D.Dot(CU::Vector2f(0.f, 1.f)));
	//CU::Matrix44f transformation = myModel->GetTransformation();


	//myModel->SetTransformation(transformation);

	//do math to set the direction of getparent->gettransformation to the angle of the 2d vector
}
