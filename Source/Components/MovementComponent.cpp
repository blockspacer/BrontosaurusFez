#include "stdafx.h"
#include "MovementComponent.h"
#include "GameObject.h"
#include "../BrontosaurusEngine/Engine.h"
#include "../CommonUtilities/Camera.h"
#include "../Game/PollingStation.h"

#include "Navmesh.h"

#include <line.h>



MovementComponent::MovementComponent()
{
	myPathPointer = nullptr;
	myMovementSpeed = 100.0f;
	myCurrentPathIndex = 0;
	myWaitUntilMoveAgianTimer = 0.0f;
	myTriangle = nullptr;
	myType = eComponentType::eMovement;
	myIsActive = true;
	myIsFollowingPath = false;
}


MovementComponent::~MovementComponent()
{
}

void MovementComponent::Update(float aDeltaTime)
{
	if (myIsActive == true)
	{
		if (myTriangle == nullptr)
		{
			CNavmesh* navmesh = PollingStation::Navmesh;
			if (navmesh != nullptr)
			{
				CU::Vector3f intersectingPoint;
				if (navmesh->IsValid(GetParent()->GetWorldPosition(), myTriangle, intersectingPoint) == false)
				{
					DL_PRINT("AI not placed on Navmesh.");
				}
				else
				{
					SComponentMessageData changeTriangleMessage;
					changeTriangleMessage.myTrianglePointer = myTriangle;
					GetParent()->NotifyComponents(eComponentMessageType::eEnterNewTriangle, changeTriangleMessage);
				}
			}
		}

		if (myWaitUntilMoveAgianTimer < 0.0f)
		{
			if (myPathPointer != nullptr)
			{
				if (myCurrentPathIndex < myPathPointer->Size())
				{
					CU::Vector3f position = GetParent()->GetWorldPosition();
					CU::Vector3f direction = myPathPointer->At(myCurrentPathIndex).myPosition - position;
					CU::Vector3f directionNormalized = direction.GetNormalized();
					CU::Vector3f movement = directionNormalized * (myMovementSpeed) * aDeltaTime;

					CU::Matrix44f& localTransform = GetParent()->GetLocalTransform();
					
						if (IsOnNavmesh(movement + position) == false)
						{
							if(myIsFollowingPath == false)
							{
								SComponentMessageData stoppedMovingMessage;
								stoppedMovingMessage.myString = "idle";
								GetParent()->NotifyComponents(eComponentMessageType::eStoppedMoving, stoppedMovingMessage);
								
								myIsFollowingPath = true;
								GetParent()->NotifyComponents(eComponentMessageType::eNotOnNavmesh, SComponentMessageData());
								return;
							
							}
						}
					
					
					movement.y = 0.0f;
					direction.y = 0.0f;
					if(myTriangle != nullptr)
					{
						CU::Vector3f dest = myTriangle->CenterPosition;
					
					}

					//DL_PRINT("triC: X %f Y %f Z%f", dest.x, dest.y, dest.z);


					//DL_PRINT("%i Speed:%f | Move x %f :z %f |	Dir x %f :z %f", myCurrentPathIndex, myMovementSpeed ,movement.x, movement.z, direction.x, direction.z);
					//DL_PRINT("myPos: X %f Z %f TargetPos: X%f Z %f", localTransform.GetPosition().x, localTransform.GetPosition().z, myPathPointer->At(myCurrentPathIndex).myPosition.x, myPathPointer->At(myCurrentPathIndex).myPosition.z);
					//DL_PRINT("dir Length: %f", direction.Length2());
					//DL_PRINT("move Length: %f", movement.Length2());
					//DL_PRINT("dist: X %f Z %f dir: X%f Z %f",direction.x, direction.z, directionNormalized.x, directionNormalized.z);
					CU::Matrix44f shitTHatMightWorkplz = localTransform; // Final names Don't change;
					CU::Matrix33f shitRotation = shitTHatMightWorkplz.GetRotation();
					shitRotation.LookAt(shitTHatMightWorkplz.GetPosition() + movement, shitTHatMightWorkplz.GetPosition());
					shitTHatMightWorkplz.SetRotation(shitRotation);
					shitTHatMightWorkplz.Move(CU::Vector3f(0.0f, 0.0f, myMovementSpeed * -aDeltaTime));
					CU::Vector3f makesNoDifferecneVector = localTransform.GetPosition() - shitTHatMightWorkplz.GetPosition();
					if (makesNoDifferecneVector.Length2() < direction.Length2())
					{
						GetParent()->NotifyComponents(eComponentMessageType::eMoving, SComponentMessageData());
						CU::Matrix33f rotationMatrix = localTransform.GetRotation();
						rotationMatrix.LookAt(localTransform.GetPosition() + movement, localTransform.GetPosition());
						localTransform.SetRotation(rotationMatrix);
						localTransform.Move(CU::Vector3f(0.0f, 0.0f, myMovementSpeed * -aDeltaTime));

						float height = 0.0f;
						//height = myTriangle->GetHeight({ localTransform.GetPosition().x,localTransform.GetPosition().z });

						if (myTriangle != nullptr)
						{
							height = myTriangle->CenterPosition.y;
						}
						
					
						CU::Vector3f newPosition = CU::Vector3f(localTransform.GetPosition().x, height, localTransform.GetPosition().z);
						localTransform.SetPosition(newPosition);
					}
					else
					{
						float height = 0.0f;

						if (myTriangle != nullptr)
						{
							height = myTriangle->GetHeight({ myPathPointer->At(myCurrentPathIndex).myPosition.x, myPathPointer->At(myCurrentPathIndex).myPosition.z });
							height = myTriangle->CenterPosition.y;
						}

						CU::Vector3f newPosition = CU::Vector3f(myPathPointer->At(myCurrentPathIndex).myPosition.x, height, myPathPointer->At(myCurrentPathIndex).myPosition.z);
						localTransform.SetPosition(newPosition);
						
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
					myWaitUntilMoveAgianTimer = 0.31f;
				}
			}
		}
		else
		{
			myWaitUntilMoveAgianTimer -= aDeltaTime;
		}
	}

}

void MovementComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	switch (aMessageType)
	{
	case eComponentMessageType::eSetPath:
		myPathPointer = aMessageData.myPathPointer;
		myCurrentPathIndex = 0;
		{
			SComponentMessageData directionData;
			directionData.myString = "walk";
			GetParent()->NotifyComponents(eComponentMessageType::eStartedMoving, directionData);
			myIsFollowingPath = true;
		}
		break;
	case eComponentMessageType::eSetPathPosition:
		myPathPointer = aMessageData.myPathPointer;
		myCurrentPathIndex = 0;
		{
			SComponentMessageData directionData;
			directionData.myString = "walk";
			GetParent()->NotifyComponents(eComponentMessageType::eStartedMoving, directionData);
			myIsFollowingPath = false;
			
		}
		break;
	case eComponentMessageType::eStopMovement:
		if (myPathPointer != nullptr)
		{
			myCurrentPathIndex = myPathPointer->Size();
			myWaitUntilMoveAgianTimer = aMessageData.myFloat;
			
			
		}
		break;
	case eComponentMessageType::eAddToMovementSpeed:
		myMovementSpeed += aMessageData.myInt;
		break;
	case eComponentMessageType::eLookAt:
	{
		GetParent()->GetLocalTransform().LookAt(aMessageData.myVector3f);
		GetParent()->GetLocalTransform().Rotate(PI, CU::Axees::Y);
		GetParent()->NotifyComponents(eComponentMessageType::eMoving, SComponentMessageData());
	}
	break;
	case eComponentMessageType::eDied:
		myIsActive = false;
		break;
	case eComponentMessageType::eRespawned:
		myIsActive = true;
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

bool MovementComponent::IsOnNavmesh(const CU::Vector3f & aPosition)
{
	float tempX1 = 0.0f;
	float tempZ1 = 0.0f;
	float tempX2 = 0.0f;
	float tempZ2 = 0.0f;
	CU::Line<float> line;
	bool isOnNavmesh = true;
	if (myTriangle != nullptr)
	{
		for (int i = 0; i < 3; ++i)
		{
			tempX1 = myTriangle->Edges[i]->FirstVertex->Position.x;
			tempZ1 = myTriangle->Edges[i]->FirstVertex->Position.z;
			tempX2 = myTriangle->Edges[i]->SecondVertex->Position.x;
			tempZ2 = myTriangle->Edges[i]->SecondVertex->Position.z;

			line.SetWith2Points({ tempX1 , tempZ1 }, { tempX2, tempZ2 });
			if (line.IsInside({ aPosition.x, aPosition.z }) != line.IsInside({ myTriangle->CenterPosition.x, myTriangle->CenterPosition.z }))
			{
				bool myIsOnnewTriangle = false;
				if (myTriangle != myTriangle->Edges[i]->Triangles[0])
				{
					if (myTriangle->Edges[i]->Triangles[0] == nullptr)
					{
						isOnNavmesh = false;
						
					}
					else
					{
						myTriangle = myTriangle->Edges[i]->Triangles[0];
						

						SComponentMessageData changeTriangleMessage;
						changeTriangleMessage.myTrianglePointer = myTriangle;
						GetParent()->NotifyComponents(eComponentMessageType::eEnterNewTriangle, changeTriangleMessage);
						myIsOnnewTriangle = true;
					}
				}

				if (myTriangle != myTriangle->Edges[i]->Triangles[1])
				{
					if (myTriangle->Edges[i]->Triangles[1] == nullptr)
					{
						isOnNavmesh = false;
						

					}
					else
					{
						myTriangle = myTriangle->Edges[i]->Triangles[1];
						

						SComponentMessageData changeTriangleMessage;
						changeTriangleMessage.myTrianglePointer = myTriangle;
						GetParent()->NotifyComponents(eComponentMessageType::eEnterNewTriangle, changeTriangleMessage);
						myIsOnnewTriangle = true;
					}

				}

				if(myIsOnnewTriangle == false)
				{
					CNavmesh* navmesh = PollingStation::Navmesh;
					if (navmesh != nullptr)
					{
						CU::Vector3f intersectingPoint;
						if (navmesh->IsValid(GetParent()->GetWorldPosition(), myTriangle, intersectingPoint) == false)
						{
							isOnNavmesh = false;
							break;
						}
					}
				}
				break;
			}
		}

		
	}

	return isOnNavmesh;
}
