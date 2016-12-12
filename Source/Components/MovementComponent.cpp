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
			
			}
			else
			{
				GetParent()->GetLocalTransform().SetPosition(myPathPointer->At(myCurrentPathIndex));
				myCurrentPathIndex++;
			}

			GetParent()->NotifyComponents(eComponentMessageType::eMoving, SComponentMessageData());
			
		}
	}
	GetParent()->GetLocalTransform().Move(CU::Vector3f(0.0f, 0.0f, 0.0f));
	GetParent()->NotifyComponents(eComponentMessageType::eMoving, SComponentMessageData());
	CU::Matrix44f cameraTransformation = CAMERA->GetTransformation();
	cameraTransformation.SetPosition(CU::Vector3f(0.0f, cameraTransformation.GetPosition().y, 0.0f));
	cameraTransformation.Move(CU::Vector3f(GetParent()->GetLocalTransform().GetPosition().x, -GetParent()->GetLocalTransform().GetPosition().z,0.0f));
	//CAMERA->SetTransformation(cameraTransformation);
}

void MovementComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	switch (aMessageType)
	{
	case eComponentMessageType::eSetPath:
		myPathPointer = aMessageData.myVector3ListPointer;
		myCurrentPathIndex = 0;
		break;
	default:
		break;
	}
}

void MovementComponent::Destroy()
{
}
