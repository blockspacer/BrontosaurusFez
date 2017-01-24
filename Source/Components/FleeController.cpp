#include "stdafx.h"
#include "FleeController.h"
#include "PollingStation.h"
#include "AIControllerComponent.h"

CFleeController::CFleeController()
{
	myControllerType = eControllerType::eFlee;
}

CFleeController::~CFleeController()
{
}

const CU::Vector2f CFleeController::Update(const CU::Time & aDeltaTime)
{
	CU::Vector2f steering;	 // if this crashes, you either haven't added a controlelr parent, or the AIcontrollerComp as a parent to your gameObject
	const CU::Vector2f hostPos = GetControllerComponent()->GetParent()->GetWorldPosition();
	const CU::Vector2f hostVelocity = GetControllerComponent()->GetVelocity();

	for (int i = 0; i < PollingStation::myThingsEnemiesShouldAvoid.Size(); ++i)
	{
		const CU::Vector2f objectPos = PollingStation::myThingsEnemiesShouldAvoid[i]->GetWorldPosition();

		if (objectPos == hostPos)
		{
			continue;
		
		}

		CU::Vector2f desiredVelocity = (hostPos - objectPos);
		float distance = desiredVelocity.Length();

		if (distance > myFleeRadius)
		{
			continue;
		
		}

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

//flytta till pollingstation istället?
void CFleeController::SetTargetsToAvoid( CU::GrowingArray<CGameObject*>* aTargetList)
{
	myObjectsToAvoid = aTargetList;
}

void CFleeController::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	switch (aMessageType)
	{
	case(eComponentMessageType::eObjectDone):
	{
		SComponentMessageData data;
		data.myComponent = this;
		GetParent()->NotifyOnlyComponents(eComponentMessageType::eAddAIBehavior, data);
	}
		break;
	default:
		break;
	}
}

void CFleeController::Destroy()
{
}
