#include "stdafx.h"
#include "AIControllerComponent.h"
#include "GameObject.h"
#include "../Game/PollingStation.h"

const CU::Vector2f StopVector = CU::Vector2f(99999, 99999);
#define SCALAR 10 // due to being our game being so large.

CAIControllerComponent::CAIControllerComponent()
{
	myControllers.Init(2);
	myMaxVelocity = 70;
	myVelocity = { 0.f,0.f };
}


CAIControllerComponent::~CAIControllerComponent()
{
	//myControllers.DeleteAll();
}

void CAIControllerComponent::AddControllerBehaviour(IController* aController)
{
	myControllers.Add(aController);
	aController->AddParent(this);
}

void CAIControllerComponent::RemoveControllerBehaviour(IController* aController)
{
	unsigned int index = myControllers.Find(aController);
	assert(index != CU::GrowingArray<int>::FoundNone && "Controller did not exist within this blend of controller");
	myControllers.DeleteCyclicAtIndex(index);
}

const CU::Vector2f & CAIControllerComponent::GetVelocity()
{
	return myVelocity;
}

void CAIControllerComponent::Update(const CU::Time& aDeltaTime)
{
	CU::Vector2f Acceleration = CU::Vector2f::Zero;
	myVelocity = CU::Vector2f::Zero;
	for (unsigned int i = 0; i < myControllers.Size(); ++i)
	{
		CU::Vector2f returnValue = myControllers[i]->Update(aDeltaTime);
		if (returnValue == StopVector)
		{
			myVelocity = CU::Vector2f::Zero;
			break;
		}
		Acceleration += returnValue;
	}

	myVelocity += Acceleration * aDeltaTime.GetSeconds();

	if (myVelocity.Length() > myMaxVelocity)
	{
		myVelocity.Normalize();
		myVelocity *= myMaxVelocity;
	}

	CU::Vector2f velocity = (myVelocity * SCALAR) * aDeltaTime.GetSeconds() ;

	if(PollingStation::playerObject != nullptr)
	{
		if (CU::Vector3f(GetParent()->GetWorldPosition() - PollingStation::playerObject->GetWorldPosition()).Length2() < 2000.0f * 2000.0f)
		{
			SComponentMessageData data;
			data.myGameObject = PollingStation::playerObject;
			GetParent()->NotifyComponents(eComponentMessageType::eSetSkillTargetObject, data);
		}
	}
	GetParent()->GetLocalTransform().Move(CU::Vector3f(velocity.x,0,velocity.y));
	GetParent()->NotifyComponents(eComponentMessageType::eMoving, SComponentMessageData());
}

void CAIControllerComponent::Destroy()
{
}

void CAIControllerComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	switch (aMessageType)
	{
	case(eComponentMessageType::eAddAIBehavior):
		AddControllerBehaviour(static_cast<IController*>(aMessageData.myComponent));
	default:
		break;
	}
}
