#include "stdafx.h"
#include "PickupComponent.h"
#include "PickupManager.h"
#include "../Components/GameObject.h"
#include "PollingStation.h"
#include "../Components/ComponentMessage.h"
#include "../Collision/ICollider.h"
//#include <iostream>

void CPickupComponent::Destroy()
{
	myManager.DestroyPickupComp(this);
}

void CPickupComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData)
{
	if (aMessageType == eComponentMessageType::eOnCollisionEnter)
	{
		//GetParent()->NotifyComponents(eComponentMessageType::eObjectiveCompleted, SComponentMessageData());
		SComponentMessageData data;
		data.myPickupComponent = this;
		aMessageData.myCollider->GetGameObject()->NotifyComponents(eComponentMessageType::ePickUp, data);
		return;
	}
}

CPickupComponent::CPickupComponent(CPickupManager& aManager) : myManager(aManager)
{
	mySuckUpRadius = 10000.0f;
	mySpeed = 1000.0f;
}

CPickupComponent::~CPickupComponent()
{
}

void CPickupComponent::Update(float aDeltaTime)
{
	float distance2 = CU::Vector3f(PollingStation::playerObject->GetToWorldTransform().GetPosition() - GetParent()->GetToWorldTransform().GetPosition()).Length2();
	if(distance2 < mySuckUpRadius * mySuckUpRadius)
	{
		CU::Matrix33f rotationMatrix = GetParent()->GetLocalTransform().GetRotation();
		rotationMatrix.LookAt(GetParent()->GetToWorldTransform().GetPosition(), PollingStation::playerObject->GetToWorldTransform().GetPosition());
		GetParent()->GetLocalTransform().SetPosition(CU::Vector3f(0.0f, 0.0f, mySpeed * aDeltaTime) * rotationMatrix + GetParent()->GetToWorldTransform().GetPosition());
		GetParent()->NotifyComponents(eComponentMessageType::eMoving, SComponentMessageData());
	}
}
