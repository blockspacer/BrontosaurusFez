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
		if (aData.myComponentTypeAdded != eComponentType::eModel) break;
	case eComponentMessageType::eMoving:
		myModel->SetTransformation(GetToWorldTransform());
		break;
	case eComponentMessageType::eStartedMoving:
		ChangeAnimation(aData.myString);
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
