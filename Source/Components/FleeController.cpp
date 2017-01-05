#include "stdafx.h"
#include "FleeController.h"
#include "PollingStation.h"

CFleeController::CFleeController()
{
	myWeight = 1.0;
	myControllerType = eControllerType::eFlee;
}

CFleeController::~CFleeController()
{
}

const CU::Vector2f CFleeController::Update(const CU::Time & aDeltaTime)
{
	CU::Vector3f playerPos = PollingStation::playerObject->GetLocalTransform().GetPosition();
	return CU::Vector2f();
}

void CFleeController::SetWeight(const float aWeight)
{
	myWeight = aWeight;
}

void CFleeController::SetTargetsToAvoid( CU::GrowingArray<CGameObject*>* aTargetList)
{
	myObjectsToAvoid = aTargetList;
}

void CFleeController::Destroy()
{
}
