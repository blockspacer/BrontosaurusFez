#include "stdafx.h"
#include "MovementComponent.h"
#include "GameObject.h"
#include "../BrontosaurusEngine/Engine.h"
#include "../CommonUtilities/Camera.h"

MovementComponent::MovementComponent()
{
	myPathPointer = nullptr;
	myMovementSpeed = 100.0f;
	myCurrentPathIndex = 0;
}


MovementComponent::~MovementComponent()
{
}

void MovementComponent::Update(float aDeltaTime)
{
	if(myPathPointer != nullptr)
	{
		if(myCurrentPathIndex < myPathPointer->Size())
		{
			CU::Vector3f position = GetParent()->GetWorldPosition();
			CU::Vector3f direction = myPathPointer->At(myCurrentPathIndex) - position;
			CU::Vector3f directionNormalized = direction.GetNormalized();
			CU::Vector3f movement = directionNormalized * myMovementSpeed * aDeltaTime;
			
			CU::Matrix44f& localTransform = GetParent()->GetLocalTransform();

			if(movement.Length2() < direction.Length2())
			{
				GetParent()->NotifyComponents(eComponentMessageType::eMoving, SComponentMessageData());

				CU::Matrix33f rotationMatrix = localTransform.GetRotation();
				rotationMatrix.LookAt(localTransform.GetPosition() + movement, localTransform.GetPosition());
				localTransform.SetRotation(rotationMatrix);
				localTransform.Move(CU::Vector3f(0.0f, 0.0f, myMovementSpeed * -aDeltaTime));
			}
			else
			{
				localTransform.SetPosition(myPathPointer->At(myCurrentPathIndex));
				myCurrentPathIndex++;

				GetParent()->NotifyComponents(eComponentMessageType::eMoving, SComponentMessageData());
			}
		}
		else
		{
			SComponentMessageData stoppedMovingMessage;
			stoppedMovingMessage.myString = "idle";
			GetParent()->NotifyComponents(eComponentMessageType::eStoppedMoving, stoppedMovingMessage);
			myPathPointer = nullptr;
		}
	}
}

void MovementComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	switch (aMessageType)
	{
	case eComponentMessageType::eSetPath:
		myPathPointer = aMessageData.myVector3ListPointer;
		myCurrentPathIndex = 0;
		{
			SComponentMessageData directionData;
			directionData.myString = "walk";
			GetParent()->NotifyComponents(eComponentMessageType::eStartedMoving, directionData);
		}
		break;
	case eComponentMessageType::eStopMovement:
		if(myPathPointer != nullptr)
		{
			myCurrentPathIndex = myPathPointer->Size();
			SComponentMessageData stoppedMovingMessage;
			stoppedMovingMessage.myString = "idle";
			GetParent()->NotifyComponents(eComponentMessageType::eStoppedMoving, stoppedMovingMessage);
		}
		break;
	default:
		break;
	}
}

void MovementComponent::Destroy()
{
}

void MovementComponent::SetMovementSpeed(const float aMovementspeed)
{
	myMovementSpeed = aMovementspeed;
}
