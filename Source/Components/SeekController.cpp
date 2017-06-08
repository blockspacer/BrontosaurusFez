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
	myShouldAvoidRight = 0;
	myPrevoiusShouldAvoidRight = 0;
	myFleeingFromDizGuyIndex = -1;
	myWaitUntilTurnCountDown = 0.0f;
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
		CU::Vector2f targetDistance = myTarget - position;
		if(targetDistance.Length2() < 10.0f)
		{
			SComponentMessageData stopData;
			stopData.myFloat = 0.1f;
			GetParent()->NotifyComponents(eComponentMessageType::eStopMovement, stopData);
		}
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
	float closestDistanceTOEnemy = 2000.0f * 2000.0f;
	int peopleThatIsTooDamnClose = 0;
	for(unsigned short i = 0; i < PollingStation::myThingsEnemiesShouldAvoid.Size(); i++)
	{
		if(GetParent() == PollingStation::myThingsEnemiesShouldAvoid[i])
		{
			continue;
		}
		
		float distanceToPlayah = CU::Vector3f(GetParent()->GetWorldPosition() - PollingStation::playerObject->GetWorldPosition()).Length2();
		float otherEnemeyDistanceToPlayah = CU::Vector3f(PollingStation::myThingsEnemiesShouldAvoid[i]->GetWorldPosition() - PollingStation::playerObject->GetWorldPosition()).Length2();
		float distanceToTOherENemyAndNOTPlayah = CU::Vector3f(PollingStation::myThingsEnemiesShouldAvoid[i]->GetWorldPosition() - GetParent()->GetWorldPosition()).Length2();
		CU::Vector3f avoidDistance = GetParent()->GetWorldPosition() - PollingStation::myThingsEnemiesShouldAvoid[i]->GetWorldPosition();
		CU::Vector2f distance(avoidDistance.x, avoidDistance.z);

		if (distance.Length2() < 50 * 50)
		{
			peopleThatIsTooDamnClose++;
			CU::Vector3f directionFromOtherEnemy = avoidDistance.GetNormalized();
			directionFromOtherEnemy *= 2.0f;
			directionFromOtherEnemy.y = 0.0f;
			GetParent()->GetLocalTransform().SetPosition(GetParent()->GetLocalTransform().GetPosition() + directionFromOtherEnemy);
		}
		else if (distance.Length2() < myEvadeRadius * myEvadeRadius)
		{
			peopleThatIsTooDamnClose++;
			CU::Vector2f direction = distance.GetNormalized();
			direction *= myMaxAcceleration;
			direction *= SCALAR;
			float avoidDifferenceLength = 100000.0f;
			if (closestDistanceTOEnemy > avoidDistance.Length2())
			{
				if (myFleeingFromDizGuyIndex != i)
				{
					closestDistanceTOEnemy = avoidDistance.Length2();
					CU::Matrix44f tempLocalTrandorm = GetParent()->GetLocalTransform();
					CU::Vector2f moveVector = myAcceleration * CU::Matrix33f::CreateRotateAroundZ(90.0f * PI / 180.0f) * 1000;
					CU::Vector3f realMoveVector(moveVector.x, tempLocalTrandorm.GetPosition().y, moveVector.y);
					tempLocalTrandorm.Move(realMoveVector);
					float distanceLeft = CU::Vector3f(tempLocalTrandorm.GetPosition() - PollingStation::myThingsEnemiesShouldAvoid[i]->GetWorldPosition()).Length2();
					tempLocalTrandorm = GetParent()->GetLocalTransform();
					moveVector = myAcceleration * CU::Matrix33f::CreateRotateAroundZ(-90.0f * PI / 180.0f) * 1000;
					realMoveVector = CU::Vector3f(moveVector.x, tempLocalTrandorm.GetPosition().y, moveVector.y);
					tempLocalTrandorm.Move(realMoveVector);
					float distanceRight = CU::Vector3f(tempLocalTrandorm.GetPosition() - PollingStation::myThingsEnemiesShouldAvoid[i]->GetWorldPosition()).Length2();
					if (distanceRight < distanceLeft)
					{
						if (myPrevoiusShouldAvoidRight == 1)
						{

							myWaitUntilTurnCountDown = 0.25f;
						}
						if (myPrevoiusShouldAvoidRight == 0)
						{
							myShouldAvoidRight = 1;
							myPrevoiusShouldAvoidRight = 2;
						}
					}
					else
					{

						if (myPrevoiusShouldAvoidRight == 2)
						{
							
							myWaitUntilTurnCountDown = 0.25f;
						}
						if (myPrevoiusShouldAvoidRight == 0)
						{
							myShouldAvoidRight = 2;
							myPrevoiusShouldAvoidRight = 1;
						}
					}
					myElapsedEvadeTime = 1.5f;
					myFleeingFromDizGuyIndex = i;
				}
			}
			if (CU::Vector2f((direction * -1) - myAcceleration).Length2() > avoidDifferenceLength * avoidDifferenceLength)
			{
				continue;
			}
			myElapsedEvadeTime = 1.5f;
			
		}
	}
	if(myWaitUntilTurnCountDown > 0.0f)
	{
		myWaitUntilTurnCountDown -= aDeltaTime.GetSeconds();
		if (myWaitUntilTurnCountDown <= 0.0f)
		{
			if(myShouldAvoidRight == 1)
			{
				myShouldAvoidRight = 2;
			}
			else
			{
				myShouldAvoidRight = 1;
			}
		}
	}
	if(myElapsedEvadeTime > 0.0f)
	{
		if(myShouldAvoidRight == 1)
		{
			myAcceleration = myAcceleration * CU::Matrix33f::CreateRotateAroundZ(60.0f * PI / 180.0f);
		}
		else if(myShouldAvoidRight == 2)
		{
			myAcceleration = myAcceleration * CU::Matrix33f::CreateRotateAroundZ(-60.0f * PI / 180.0f);
		}
	}
	else
	{
		myFleeingFromDizGuyIndex = -1;
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
	case(eComponentMessageType::eSpawning):
	{
		myStartPosition = GetParent()->GetWorldPosition();
		myStartPosition.y = GetParent()->GetWorldPosition().z;
	}
	break;
	default:
		break;
	}
}

void CSeekController::Destroy()
{
}
