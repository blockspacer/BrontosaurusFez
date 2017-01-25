#include "stdafx.h"
#include "FormationController.h"
#include "GameObject.h"
#include "../Game/PollingStation.h"
#include "AIControllerComponent.h"
#include "../Game/MasterAI.h"

#define SCALAR 3

FormationController::FormationController()
{
	myTargetRadius = 0;
	myTarget = CU::Vector2f::Zero;//PollingStation::playerObject->GetWorldPosition();
	myAcceleration = CU::Vector2f::Zero;
	myControllerType = eControllerType::eArrive;
	myHaveBeenCalledForHelp = false;
	myCallForHelpRadius = 0.0f;
	myFormationIndex = -1;
}


FormationController::~FormationController()
{
}

const CU::Vector2f FormationController::Update(const CU::Time& aDeltaTime)
{
	if (PollingStation::playerObject == nullptr)
	{
		return CU::Vector2f::Zero;
	}

	myTarget = CU::Vector2f(PollingStation::playerObject->GetWorldPosition().x, PollingStation::playerObject->GetWorldPosition().z);
	myTarget = CalculateFormationPosition(myTarget);
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
	if (myHaveBeenCalledForHelp == false)
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

void FormationController::SetWeight(const float aWeight)
{
	myWeight = aWeight;
}

void FormationController::SetTarget(const CU::Vector2f & aPosition)
{
	myTarget = aPosition;
}

void FormationController::SetMaxSpeed(const float aMaxSpeed)
{
	myMaxSpeed = aMaxSpeed;
}

void FormationController::SetMaxAcceleration(const float aMaxAcceleration)
{
	myMaxAcceleration = aMaxAcceleration;
}

void FormationController::SetTargetRadius(const float aTargetRadius)
{
	myTargetRadius = aTargetRadius;
}

void FormationController::SetSlowDownRadius(const float aSlowdownRadius)
{
	mySlowdownRadius = aSlowdownRadius;
}

void FormationController::SetAggroRange(const float aRange)
{
	myAggroRange = aRange;
}

void FormationController::SetCallForHelpRange(const float aRange)
{
	myCallForHelpRadius = aRange;
}

void FormationController::CallForHelp()
{
	myHaveBeenCalledForHelp = true;
}

CU::Vector2f& FormationController::CalculateFormationPosition(const CU::Vector2f& aTargetPositon)
{
	const float formationDistance = 500.0f;
	CU::Vector2f formationPosition = aTargetPositon;
	if(myFormationIndex <= 0)
	{
		return formationPosition;
	}

	CU::Vector2f targetPosition(PollingStation::playerObject->GetWorldPosition().x, PollingStation::playerObject->GetWorldPosition().z);
	CU::Vector2f position = CU::Vector2f(myController->GetParent()->GetWorldPosition().x, myController->GetParent()->GetWorldPosition().z);
	CU::Vector2f direction = targetPosition - position;
	direction.Normalize();
	direction -= direction * formationDistance;
	direction += direction * formationDistance * static_cast<short>((myFormationIndex -1) / 2.0f);
	formationPosition += direction;
	direction = targetPosition - position;
	if(myFormationIndex % 2 == 1)
	{
		direction = direction * CU::Matrix33f::CreateRotateAroundZ(PI / 2.0f);
	
	}
	else
	{
		direction = direction * CU::Matrix33f::CreateRotateAroundZ(-PI / 2.0f);
	}
	direction.Normalize();
	direction *= formationDistance;
	formationPosition += direction;
	return formationPosition;
}

void FormationController::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
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
		myFormationIndex = static_cast<short>(aMessageData.myUShort);
	}
	default:
		break;
	}
}

void FormationController::Destroy()
{
}
