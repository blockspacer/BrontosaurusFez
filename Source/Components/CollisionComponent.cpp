#include "stdafx.h"
#include "CollisionComponent.h"

#include "../Collision/ICollider.h"

CCollisionComponent::CCollisionComponent(ICollider* aCollider)
	: myCollider(aCollider)
{
	if (myCollider == nullptr)
	{
		assert(!"Collision component got collider that is NULL");
		return;
	}
	
	myCollider->InitCallbackFunctions(
		std::bind(&CCollisionComponent::OnCollisionEnter, this, std::placeholders::_1),
		std::bind(&CCollisionComponent::OnCollisionExit, this, std::placeholders::_1)
	);
}

CCollisionComponent::~CCollisionComponent()
{
	SAFE_DELETE(myCollider);
}

void CCollisionComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData)
{
	if (myCollider == nullptr)
	{
		assert(!"Collision component has collider that is NULL");
		return;
	}

	switch (aMessageType)
	{
	case eComponentMessageType::eAddComponent:
		myCollider->SetGameObject(GetParent());
		myCollider->SetPosition(GetParent()->GetWorldPosition());
		break;
	case eComponentMessageType::eMoving:
		myCollider->SetPosition(GetParent()->GetWorldPosition());
		break;
	case eComponentMessageType::eSetIsColliderActive:
		if (aMessageData.myBool == true)
		{
			ActivateCollider();
		}
		else
		{
			DeactivateCollider();
		}
	break;
	case eComponentMessageType::eDied:
		myCollider->Deactivate();
		break;
	}
}


void CCollisionComponent::Destroy()
{
	DL_ASSERT("not implemented");
}

void CCollisionComponent::ActivateCollider()
{
	myCollider->Activate();
}

void CCollisionComponent::DeactivateCollider()
{
	myCollider->Deactivate();
}

void CCollisionComponent::OnCollisionEnter(ICollider* aCollider)
{
	//DL_PRINT("%s collided with %s", GetParent()->GetName().c_str(), aCollider->GetGameObject()->GetName().c_str());
	SComponentMessageData data;
	data.myCollider = aCollider;
	GetParent()->NotifyComponents(eComponentMessageType::eOnCollisionEnter, data);
}

void CCollisionComponent::OnCollisionExit(ICollider* aCollider)
{
	//DL_PRINT("%s stopped colliding with %s", GetParent()->GetName().c_str(), aCollider->GetGameObject()->GetName().c_str());
	SComponentMessageData data;
	data.myCollider = aCollider;
	GetParent()->NotifyComponents(eComponentMessageType::eOnCollisionExit, data);
}

void CCollisionComponent::SetColliderType(const eColliderType aColliderType)
{
	myCollider->SetColliderType(aColliderType);
}

void CCollisionComponent::AddCollidsWith(const unsigned int aColliderTypes)
{
	myCollider->AddCollidsWith(aColliderTypes);
}