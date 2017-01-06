#include "stdafx.h"
#include "FleeController.h"
#include "PollingStation.h"
#include "AIControllerComponent.h"

CFleeController::CFleeController()
{
	myWeight = 5.0;
	myControllerType = eControllerType::eFlee;
	myFleeRadius = 100.f;
}

CFleeController::~CFleeController()
{
}

const CU::Vector2f CFleeController::Update(const CU::Time & aDeltaTime)
{
	CU::Vector2f steering;	 // if this crashes, you either haven't added a controlelr parent, or the AIcontrollerComp as a parent to your gameObject
	CU::Vector2f hostPos = GetControllerComponent()->GetParent()->GetWorldPosition();
	CU::Vector2f hostVelocity = GetControllerComponent()->GetVelocity();
	
	for (int i = 0; i < myObjectsToAvoid->Size(); ++i)
	{
		CU::Vector2f objectPos = myObjectsToAvoid->At(i)->GetWorldPosition();

		if (objectPos == hostPos)
			continue;

		CU::Vector2f desiredVelocity = (hostPos - objectPos);
		float distance = desiredVelocity.Length();

		if (distance > myFleeRadius)
			continue;

		steering += desiredVelocity - hostVelocity;
	}

	return steering * myWeight;
}

void CFleeController::SetWeight(const float aWeight)
{
	myWeight = aWeight;
}

void CFleeController::SetFleeRadius(float aRadius)
{
	myFleeRadius = aRadius;
}

//flytta till pollingstation ist�llet?
void CFleeController::SetTargetsToAvoid( CU::GrowingArray<CGameObject*>* aTargetList)
{
	myObjectsToAvoid = aTargetList;
}

void CFleeController::Destroy()
{
}
