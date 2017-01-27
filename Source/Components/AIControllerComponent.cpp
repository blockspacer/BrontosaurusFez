#include "stdafx.h"
#include "AIControllerComponent.h"
#include "GameObject.h"
#include "../Game/PollingStation.h"


#include "Navmesh.h"
#include <line.h>


const CU::Vector2f StopVector = CU::Vector2f(99999, 99999);
#define SCALAR 10 // due to being our game being so large.

CAIControllerComponent::CAIControllerComponent()
{
	myControllers.Init(2);
	myTriangle = nullptr;
	myMaxVelocity = 70;
	myVelocity = { 0.f,0.f };
	myType = eComponentType::eAIController;
}


CAIControllerComponent::~CAIControllerComponent()
{
	//myControllers.DeleteAll();
}

void CAIControllerComponent::AddControllerBehaviour(IController* aController)
{
	myControllers.Add(aController);
	aController->AddParent(this);
}

void CAIControllerComponent::RemoveControllerBehaviour(IController* aController)
{
	unsigned int index = myControllers.Find(aController);
	assert(index != CU::GrowingArray<int>::FoundNone && "Controller did not exist within this blend of controller");
	myControllers.DeleteCyclicAtIndex(index);
}

const CU::Vector2f & CAIControllerComponent::GetVelocity()
{
	return myVelocity;
}

void CAIControllerComponent::Update(const CU::Time& aDeltaTime)
{
	if (myTriangle == nullptr)
	{
		CNavmesh* navmesh = PollingStation::Navmesh;
		if (navmesh != nullptr)
		{
			CU::Vector3f intersectingPoint;
			if (navmesh->IsValid(GetParent()->GetWorldPosition(), myTriangle, intersectingPoint) == false )
			{
				DL_PRINT("AI not placed on Navmesh.");
			}
		}
	}

	CU::Vector2f Acceleration = CU::Vector2f::Zero;
	myVelocity = CU::Vector2f::Zero;
	for (unsigned int i = 0; i < myControllers.Size(); ++i)
	{
		CU::Vector2f returnValue = myControllers[i]->Update(aDeltaTime);
		if (returnValue == StopVector)
		{
			myVelocity = CU::Vector2f::Zero;
			break;
		}
		Acceleration += returnValue;
	}

	if (Acceleration == CU::Vector2f::Zero)
	{
		return;
	}

	myVelocity += Acceleration;

	if (myVelocity.Length() > myMaxVelocity)
	{
		myVelocity.Normalize();
		myVelocity *= myMaxVelocity;
	}

	CU::Vector2f velocity = (myVelocity * SCALAR) * aDeltaTime.GetSeconds() ;

	if(PollingStation::playerObject != nullptr)
	{
		if (CU::Vector3f(GetParent()->GetWorldPosition() - PollingStation::playerObject->GetWorldPosition()).Length2() < 2000.0f * 2000.0f)
		{
			SComponentMessageData data;
			data.myGameObject = PollingStation::playerObject;
			GetParent()->NotifyComponents(eComponentMessageType::eSetSkillTargetObject, data);
		}
	}



	float tempX1 = 0.0f;
	float tempZ1 = 0.0f;
	float tempX2 = 0.0f;
	float tempZ2 = 0.0f;
	CU::Vector3f newPosition = GetParent()->GetWorldPosition() + CU::Vector3f(velocity.x, 0, velocity.y);
	CU::Line<float> line;
	bool isOnNavmesh = true;
	for (int i = 0; i < 3; ++i)
	{
		tempX1 = myTriangle->Edges[i]->FirstVertex->Position.x;
		tempZ1 = myTriangle->Edges[i]->FirstVertex->Position.z;
		tempX2 = myTriangle->Edges[i]->SecondVertex->Position.x;
		tempZ2 = myTriangle->Edges[i]->SecondVertex->Position.z;

		line.SetWith2Points({ tempX1 , tempZ1 }, { tempX2, tempZ2 });
		if (line.IsInside({ newPosition.x, newPosition.z }) != line.IsInside({ myTriangle->CenterPosition.x, myTriangle->CenterPosition.z }))
		{
			if (myTriangle != myTriangle->Edges[i]->Triangles[0])
			{
				if (myTriangle->Edges[i]->Triangles[0] == nullptr)
				{
					isOnNavmesh = false;
					break;
				}
				myTriangle = myTriangle->Edges[i]->Triangles[0];
				DL_PRINT("Changing triangle!");
			}
			else if (myTriangle != myTriangle->Edges[i]->Triangles[1])
			{
				if (myTriangle->Edges[i]->Triangles[1] == nullptr)
				{
					isOnNavmesh = false;
					break;

				}
				myTriangle = myTriangle->Edges[i]->Triangles[1];
				DL_PRINT("Changing triangle!");

			}
			else
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

	if (isOnNavmesh == true)
	{
		float height = myTriangle->GetHeight({newPosition.x, newPosition.z});
		myPath.RemoveAll();


		CPath::SWaypoint wayPoint;
		wayPoint.myPosition = { newPosition.x, height,newPosition.z };
		myPath.Add(wayPoint);


		eComponentMessageType type = eComponentMessageType::eSetPath;
		SComponentMessageData data;
		data.myPathPointer = &myPath;
		GetParent()->NotifyComponents(type, data);
		GetParent()->NotifyComponents(eComponentMessageType::eMoving, SComponentMessageData());
	}
	else
	{
		int br = 0;
		br++;
	}

}

void CAIControllerComponent::Destroy()
{
}

void CAIControllerComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	switch (aMessageType)
	{
	case(eComponentMessageType::eAddAIBehavior):
		AddControllerBehaviour(static_cast<IController*>(aMessageData.myComponent));
		break;
	case(eComponentMessageType::eObjectDone):
		PollingStation::myThingsEnemiesShouldAvoid.Add(GetParent());
		DL_PRINT("Compo Id AI %u", GetId());
		break;
	default:
		break;
	}
}
