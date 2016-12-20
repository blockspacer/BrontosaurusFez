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
		std::bind(&CCollisionComponent::OnCollisionUpdate, this, std::placeholders::_1),
		std::bind(&CCollisionComponent::OnCollisionExit, this, std::placeholders::_1)
	);
}

CCollisionComponent::~CCollisionComponent()
{
	SAFE_DELETE(myCollider);
}

void CCollisionComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData& /*aMessageData*/)
{
	if (myCollider == nullptr)
	{
		assert(!"Collision component has collider that is NULL");
		return;
	}

	switch (aMessageType)
	{
	case eComponentMessageType::eMoving:
		myCollider->SetPosition(GetParent()->GetWorldPosition());
		break;
	}
}

void CCollisionComponent::Destroy()
{
	DL_ASSERT("not implemented");
}

void CCollisionComponent::OnCollisionEnter(ICollider* /*aCollider*/)
{
}

void CCollisionComponent::OnCollisionUpdate(ICollider* /*aCollider*/)
{
}

void CCollisionComponent::OnCollisionExit(ICollider* /*aCollider*/)
{
}
