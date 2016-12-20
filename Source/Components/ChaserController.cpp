#include "stdafx.h"
#include "ChaserController.h"
#include "GameObject.h"
#include "../Game/PollingStation.h"
#include "AIControllerComponent.h"

CChaserController::CChaserController()
{
	myMaxSpeed = 0;
	myWeight = 1.0f;
	myTargetRadius = 0;
	mySlowdownRadius = 0;
	myMaxAcceleration = 0;
	myTarget = PollingStation::playerObject->GetWorldPosition();
	myAcceleration = CU::Vector2f::Zero;
	myControllerType = eControllerType::eArriver;
}


CChaserController::~CChaserController()
{
}

const CU::Vector2f CChaserController::Update(const CU::Time& aDeltaTime)
{
	myTarget = CU::Vector2f(PollingStation::playerObject->GetWorldPosition().x, PollingStation::playerObject->GetWorldPosition().y);
	CU::Vector2f position = CU::Vector2f(myController->GetParent()->GetWorldPosition().x, myController->GetParent()->GetWorldPosition().y);
	CU::Vector2f targetVelocity = CU::Vector2f::Zero;
	targetVelocity = myTarget - position;

	float distance = targetVelocity.Length();

	if (distance < myTargetRadius)
	{
		myWeight = 0;
		return CU::Vector2f::Zero;
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

void CChaserController::SetWeight(const float aWeight)
{
	myWeight = aWeight;
}

void CChaserController::SetTarget(const CU::Vector2f & aPosition)
{
	myWeight = 1.0f;
	myTarget = aPosition;
}

void CChaserController::SetMaxSpeed(const float aMaxSpeed)
{
	myMaxSpeed = aMaxSpeed;
}

void CChaserController::SetMaxAcceleration(const float aMaxAcceleration)
{
	myMaxAcceleration = aMaxAcceleration;
}

void CChaserController::SetTargetRadius(const float aTargetRadius)
{
	myTargetRadius = aTargetRadius;
}

void CChaserController::SetSlowDownRadius(const float aSlowdownRadius)
{
	mySlowdownRadius = aSlowdownRadius;
}

void CChaserController::Destroy()
{
}
