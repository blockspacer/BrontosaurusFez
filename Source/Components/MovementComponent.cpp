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
			CU::Vector2f position(GetParent()->GetWorlPosition().x, GetParent()->GetWorlPosition().y);
			CU::Vector2f direction = myPathPointer->At(myCurrentPathIndex) - position;
			CU::Vector2f directionNormalized = direction.GetNormalized();
			CU::Vector2f movement = directionNormalized * myMovementSpeed * aDeltaTime;


			if(movement.Length2() < direction.Length2())
			{
				GetParent()->GetLocalTransform().Move(CU::Vector3f(movement.x, movement.y, 0.0));
			
			}
			else
			{
				GetParent()->GetLocalTransform().SetPosition(CU::Vector3f(myPathPointer->At(myCurrentPathIndex).x, myPathPointer->At(myCurrentPathIndex).y, 0.0f));
				myCurrentPathIndex++;
			}

			GetParent()->NotifyComponents(eComponentMessageType::eMoving, SComponentMessageData());
			CU::Matrix44f cameraTransformation = CAMERA->GetTransformation();
			cameraTransformation.SetPosition(CU::Vector3f(0.0f, 0.0f, cameraTransformation.GetPosition().z));
			cameraTransformation.Move(CU::Vector3f(GetParent()->GetLocalTransform().GetPosition().x, GetParent()->GetLocalTransform().GetPosition().y, 0.0f));
			CAMERA->SetTransformation(cameraTransformation);
		}
	}
}

void MovementComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	switch (aMessageType)
	{
	case eComponentMessageType::eSetPath:
		myPathPointer = aMessageData.myVector2ListPointer;
		myCurrentPathIndex = 0;
		break;
	default:
		break;
	}
}

void MovementComponent::Destroy()
{
}
