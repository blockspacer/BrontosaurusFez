#include "stdafx.h"
#include "AIControllerComponent.h"
#include "GameObject.h"

CAIControllerComponent::CAIControllerComponent()
{
	myControllers.Init(2);
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
		Acceleration += myControllers[i]->Update(aDeltaTime);
	}
	myVelocity += Acceleration * aDeltaTime.GetSeconds();
	CU::Vector2f velocity = myVelocity * aDeltaTime.GetSeconds();

	GetParent()->GetToWorldTransform().Move(CU::Vector3f(myVelocity.x,0,myVelocity.y));
}

void CAIControllerComponent::Destroy()
{
}