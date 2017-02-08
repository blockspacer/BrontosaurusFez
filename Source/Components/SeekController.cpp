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
	myFormationIndex = -1;
	myEvadeRadius = 150.0f;
	myElapsedEvadeTime = 0.0f;
	myShouldGoBackToStatPosition = false;
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
	myTarget = CalculateFormationPosition(myTarget);
	CU::Vector2f position = CU::Vector2f(myController->GetParent()->GetWorldPosition().x, myController->GetParent()->GetWorldPosition().z);
	CU::Vector2f targetVelocity = CU::Vector2f::Zero;
	if (myShouldGoBackToStatPosition == true)
	{
		myTarget = myStartPosition;
		DL_PRINT("Goinf Back");
	}
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
	else if (distance > (myAggroRange * 3))
	{
		myHaveBeenCalledForHelp = false;
		return CU::Vector2f::Zero;
	}
	GetParent()->GetLocalTransform().Lerp(GetParent()->GetLocalTransform().CreateLookAt(CU::Vector3f(myTarget.x, GetParent()->GetWorldPosition().y, myTarget.y) * -1), 0.01f);
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
	myElapsedEvadeTime -= aDeltaTime.GetSeconds();
	for(unsigned short i = 0; i < PollingStation::myThingsEnemiesShouldAvoid.Size(); i++)
	{
		CU::Vector3f avoidDistance = GetParent()->GetWorldPosition() - PollingStation::myThingsEnemiesShouldAvoid[i]->GetWorldPosition();
		CU::Vector2f distance(avoidDistance.x, avoidDistance.z);
		if(distance.Length2() < myEvadeRadius * myEvadeRadius)
		{
			CU::Vector2f direction = distance.GetNormalized();
			direction *= myMaxAcceleration;
			direction *= SCALAR;
			float avoidDifferenceLength = 35.0f;
			if (CU::Vector2f((direction * -1) - myAcceleration).Length2() > avoidDifferenceLength * avoidDifferenceLength)
			{
				continue;
			}
			myElapsedEvadeTime = 0.5f;
			break;
		}
	}
	if(myElapsedEvadeTime > 0.0f)
	{
		myAcceleration = myAcceleration * CU::Matrix33f::CreateRotateAroundZ(90.0f * PI / 180.0f);
	}
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

void CSeekController::CalledUponForHelp()
{
	myHaveBeenCalledForHelp = true;
	CU::Vector2f targetPosition(PollingStation::playerObject->GetWorldPosition().x, PollingStation::playerObject->GetWorldPosition().z);
	CU::Vector2f position = CU::Vector2f(myController->GetParent()->GetWorldPosition().x, myController->GetParent()->GetWorldPosition().z);
	myFormationDirection = targetPosition - position;
	myFormationDirection.Normalize();
	myFormationDirection *= -1;
}

CU::Vector2f& CSeekController::CalculateFormationPosition(const CU::Vector2f& aTargetPositon)
{
	if(myFormationIndex <= 0)
	{
		return aTargetPositon + CU::Vector2f::Zero;
	}
	const float formationDistance = 50.0f;
	float rotationAmount = PI / 16.0f;
	if(myFormationIndex % 2 == 0)
	{
		rotationAmount *= -1;
	}
	if(myFormationIndex == 0)
	{
		rotationAmount = 0;
	}
	short angleMultiplier = static_cast<short>((myFormationIndex - 1) / 2) + 1;
	CU::Vector2f formationDirectionRotated = myFormationDirection * CU::Matrix33f::CreateRotateAroundZ(rotationAmount * angleMultiplier);
	formationDirectionRotated *= formationDistance;
	return aTargetPositon + formationDirectionRotated;
	//Old Spin to win Formation Code;
	/*const float formationDistance = 10.0f;
	CU::Vector2f formationPosition = aTargetPositon;
	if (myFormationIndex <= 0)
	{
		return formationPosition;
	}

	CU::Vector2f targetPosition(PollingStation::playerObject->GetWorldPosition().x, PollingStation::playerObject->GetWorldPosition().z);
	CU::Vector2f position = CU::Vector2f(myController->GetParent()->GetWorldPosition().x, myController->GetParent()->GetWorldPosition().z);
	CU::Vector2f direction = targetPosition - position;
	direction.Normalize();
	direction -= direction * formationDistance;
	direction += direction * formationDistance * static_cast<short>((myFormationIndex - 1) / 2.0f);
	formationPosition += direction;
	direction = targetPosition - position;
	if (myFormationIndex % 2 == 1)
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
	return formationPosition;*/
}

void CSeekController::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{

	switch (aMessageType)
	{
	case(eComponentMessageType::eObjectDone):
	{
		SComponentMessageData data;
		data.myComponent = this;
		GetParent()->NotifyComponents(eComponentMessageType::eAddAIBehavior, data);
		myStartPosition = GetParent()->GetWorldPosition();
		myStartPosition.y = GetParent()->GetWorldPosition().z;
	}
	break;
	case(eComponentMessageType::eCalledForHelp):
	{
		CalledUponForHelp();
		myFormationIndex = static_cast<short>(aMessageData.myUShort);
	}
	break;
	case(eComponentMessageType::eEnemyReturnToSpawnPoint):
	{
		myShouldGoBackToStatPosition = true;
	}
	break;
	case(eComponentMessageType::eEnemyStartChaseAgain):
	{
		myShouldGoBackToStatPosition = false;
	}
	break;
	default:
		break;
	}
}

void CSeekController::Destroy()
{
}
