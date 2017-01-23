#include "stdafx.h"
#include "SeekController.h"
#include "GameObject.h"
#include "../Game/PollingStation.h"
#include "AIControllerComponent.h"
#include "../Game/MasterAI.h"

#define SCALAR 3

CSeekController::CSeekController()
{
	myTargetRadius = 0;
	myTarget = CU::Vector2f::Zero;//PollingStation::playerObject->GetWorldPosition();
	myAcceleration = CU::Vector2f::Zero;
	myControllerType = eControllerType::eArrive;
	myType = eComponentType::eSeekController;
	myHaveBeenCalledForHelp = false;
	myCallForHelpRadius = 0.0f;
}


CSeekController::~CSeekController()
{
}

const CU::Vector2f CSeekController::Update(const CU::Time& aDeltaTime)
{
	if (PollingStation::playerObject == nullptr)
	{
		return CU::Vector2f::Zero;
	}

	myTarget = CU::Vector2f(PollingStation::playerObject->GetWorldPosition().x, PollingStation::playerObject->GetWorldPosition().z);
	CU::Vector2f position = CU::Vector2f(myController->GetParent()->GetWorldPosition().x, myController->GetParent()->GetWorldPosition().z);
	CU::Vector2f targetVelocity = CU::Vector2f::Zero;
	targetVelocity = myTarget - position;


	float distance = targetVelocity.Length();

	if (distance > myAggroRange && myHaveBeenCalledForHelp == false)
	{
		return CU::Vector2f::Zero;
	}
	else if (distance < myTargetRadius && myHaveBeenCalledForHelp == false)
	{
		return CU::Vector2f(99999, 99999); // ?
	}
	else if (distance < mySlowdownRadius)
	{
		float speed = myMaxSpeed;
		speed = myMaxSpeed * distance / mySlowdownRadius;
		targetVelocity.Normalize() *= speed;
	}
	GetParent()->GetLocalTransform().Lerp(GetParent()->GetLocalTransform().CreateLookAt(PollingStation::playerObject->GetLocalTransform().GetPosition() * -1), 0.01f);
	if(myHaveBeenCalledForHelp == false)
	{
		myHaveBeenCalledForHelp = true;
		CMasterAI::GetInstance().CallForHelp(GetParent(), myCallForHelpRadius);
	}
	CU::Vector2f acceleration;
	acceleration = targetVelocity;
	if (acceleration.Length() > myMaxAcceleration)
	{
		acceleration.Normalize() *= myMaxAcceleration;
	}
	myAcceleration = acceleration * SCALAR;
	return myAcceleration * myWeight;
}

void CSeekController::SetWeight(const float aWeight)
{
	myWeight = aWeight;
}

void CSeekController::SetTarget(const CU::Vector2f & aPosition)
{
	myTarget = aPosition;
}

void CSeekController::SetMaxSpeed(const float aMaxSpeed)
{
	myMaxSpeed = aMaxSpeed;
}

void CSeekController::SetMaxAcceleration(const float aMaxAcceleration)
{
	myMaxAcceleration = aMaxAcceleration;
}

void CSeekController::SetTargetRadius(const float aTargetRadius)
{
	myTargetRadius = aTargetRadius;
}

void CSeekController::SetSlowDownRadius(const float aSlowdownRadius)
{
	mySlowdownRadius = aSlowdownRadius;
}

void CSeekController::SetAggroRange(const float aRange)
{
	myAggroRange = aRange;
}

void CSeekController::SetCallForHelpRange(const float aRange)
{
	myCallForHelpRadius = aRange;
}

void CSeekController::CallForHelp()
{
	myHaveBeenCalledForHelp = true;
}

void CSeekController::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{

	switch (aMessageType)
	{
	case(eComponentMessageType::eObjectDone):
	{
		SComponentMessageData data;
		data.myComponent = this;
		NotifyParent(eComponentMessageType::eAddAIBehavior, data);
	}
		break;
	case(eComponentMessageType::eCalledForHelp):
	{
		myHaveBeenCalledForHelp = true;
	}
	default:
		break;
	}
}

void CSeekController::Destroy()
{
}
