#include "stdafx.h"
#include "SeekController.h"
#include "GameObject.h"
#include "../Game/PollingStation.h"
#include "AIControllerComponent.h"

CSeekController::CSeekController()
{
	myMaxSpeed = 0;
	myWeight = 1.0f;
	myTargetRadius = 0;
	mySlowdownRadius = 0;
	myMaxAcceleration = 0;
	myTarget = PollingStation::playerObject->GetWorldPosition();
	myAcceleration = CU::Vector2f::Zero;
	myControllerType = eControllerType::eArrive;
}


CSeekController::~CSeekController()
{
}

const CU::Vector2f CSeekController::Update(const CU::Time& aDeltaTime)
{
	myTarget = CU::Vector2f(PollingStation::playerObject->GetWorldPosition().x, PollingStation::playerObject->GetWorldPosition().z);
	CU::Vector2f position = CU::Vector2f(myController->GetParent()->GetWorldPosition().x, myController->GetParent()->GetWorldPosition().z);
	CU::Vector2f targetVelocity = CU::Vector2f::Zero;
	targetVelocity = myTarget - position;

	float distance = targetVelocity.Length();

	if (distance < myTargetRadius)
	{
		return CU::Vector2f(99999, 99999);
	}
	float speed = myMaxSpeed;
	
	if (distance < mySlowdownRadius)
	{
		speed = myMaxSpeed * distance / mySlowdownRadius;
	}
	
	targetVelocity.Normalize() *= speed;
	
	CU::Vector2f acceleration;
	acceleration = targetVelocity - myVelocity;
	if (acceleration.Length() > myMaxAcceleration)
	{
		acceleration.Normalize() *= myMaxAcceleration;
	}
	myAcceleration = acceleration;
	return myAcceleration * myWeight;
}

void CSeekController::SetWeight(const float aWeight)
{
	myWeight = aWeight;
}

void CSeekController::SetTarget(const CU::Vector2f & aPosition)
{
	myWeight = 1.0f;
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

void CSeekController::Destroy()
{
}
