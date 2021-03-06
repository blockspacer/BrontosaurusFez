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
	myType = eComponentType::eModelDebug;
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
	case eComponentMessageType::eDied:

		ChangeAnimation("die");
		myModel->SetAnimationLooping(false);
		myModel->SetHighlightIntencity(0.0f);
		break;
	case eComponentMessageType::eChangeFBXToDead:
	{
		CU::Matrix44f orientation = myModel->GetTransformation();
		myModel->SetVisibility(false); // Much easier to just temp set to invis.
		
		CModelComponent* newComp =
			CModelComponentManager::GetInstance().CreateComponent(aData.myString);
		newComp->myModel->myTransformation = orientation;
		GetParent()->AddComponent(&*newComp);
		GetParent()->RemoveComponent(this); // is deleted in compmgr when level changes.
		break;
	}
	case eComponentMessageType::eAddComponent:
		if (aData.myComponentTypeAdded != eComponentType::eModel) break; //else: fall through
	case eComponentMessageType::eMoving:
		myModel->SetTransformation(GetToWorldTransform());
		break;
	case eComponentMessageType::eStartedMoving:
		ChangeAnimation(aData.myString);
		break;
	//case eComponentMessageType::eChangedDirection:
	//	ChangeDirection(aData.myVector2f);
	//	break;
	case eComponentMessageType::eStoppedMoving:
		ChangeAnimation(aData.myString);
		break;
	case eComponentMessageType::eSetVisibility:
		SetVisibility(aData.myBool);
		break;
	case eComponentMessageType::eBasicAttack:
		ChangeAnimation(aData.myString);
		break;
	case eComponentMessageType::eRespawned:
		ChangeAnimation("idle");
		myModel->SetVisibility(true);
		myModel->SetAnimationLooping(true);
		break;
	case eComponentMessageType::eSetHighlight:
		myModel->SetHighlightIntencity(aData.myFloat);
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

//void CModelComponent::ChangeDirection(const CU::Vector2f& aDirection2D)
//{
//}
