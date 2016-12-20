#include "stdafx.h"
#include "AIControllerComponent.h"
#include "GameObject.h"

const CU::Vector2f StopVector = CU::Vector2f(99999, 99999);

CAIControllerComponent::CAIControllerComponent()
{
	myControllers.Init(2);
	myMaxVelocity = 70;
}


CAIControllerComponent::~CAIControllerComponent()
{
	myControllers.DeleteAll();
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

void CAIControllerComponent::Update(const CU::Time& aDeltaTime)
{
	CU::Vector2f Acceleration = CU::Vector2f::Zero;
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


	CU::Vector2f velocity = myVelocity * aDeltaTime.GetSeconds();

	GetParent()->GetLocalTransform().Move(CU::Vector3f(velocity.x,0,velocity.y));
	GetParent()->NotifyComponents(eComponentMessageType::eMoving, SComponentMessageData());
}

void CAIControllerComponent::Destroy()
{
}