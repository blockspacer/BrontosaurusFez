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
			CU::Vector3f position = GetParent()->GetWorlPosition();
			CU::Vector3f direction = myPathPointer->At(myCurrentPathIndex) - position;
			CU::Vector3f directionNormalized = direction.GetNormalized();
			CU::Vector3f movement = directionNormalized * myMovementSpeed * aDeltaTime;
			

			if(movement.Length2() < direction.Length2())
			{
				GetParent()->GetLocalTransform().Move(movement);

				SComponentMessageData deltaMovement;
				deltaMovement.myVector3f = movement;
				GetParent()->NotifyComponents(eComponentMessageType::eMoving, deltaMovement);


				//CU::Matrix33f rotationMatrix = GetParent()->GetLocalTransform().GetRotation();
				//rotationMatrix.LookAt(GetParent()->GetLocalTransform().GetPosition(), GetParent()->GetLocalTransform().GetPosition() + movement);
				//GetParent()->GetLocalTransform().SetRotation(rotationMatrix);
			}
			else
			{
				GetParent()->GetLocalTransform().SetPosition(myPathPointer->At(myCurrentPathIndex));

				SComponentMessageData deltaMovement;
				deltaMovement.myVector3f = direction;
				GetParent()->NotifyComponents(eComponentMessageType::eMoving, deltaMovement);

				myCurrentPathIndex++;
			}
		}
		else
		{
			SComponentMessageData stoppedMovingMessage;
			stoppedMovingMessage.myString = "idle";
			GetParent()->NotifyComponents(eComponentMessageType::eStoppedMoving, stoppedMovingMessage);
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
	default:
		break;
	}
}

void MovementComponent::Destroy()
{
}
