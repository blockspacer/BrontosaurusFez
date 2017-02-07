#include "stdafx.h"
#include "AIControllerComponent.h"
#include "GameObject.h"
#include "../Game/PollingStation.h"


#include "Navmesh.h"

const CU::Vector2f StopVector = CU::Vector2f(99999, 99999);
#define SCALAR 10 // due to being our game being so large.

CAIControllerComponent::CAIControllerComponent()
{
	myControllers.Init(2);
	myMaxVelocity = 70;
	myVelocity = { 0.f,0.f };
	myType = eComponentType::eAIController;
	myElapsedPathFindingCountdown = 0.0f;
	myTrianglePointer = nullptr;
	myIsActive = true;
	myIsPlayerAlive = true;
}


CAIControllerComponent::~CAIControllerComponent()
{
	myTrianglePointer = nullptr;
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
	if(myIsActive == true)
	{
		if (myElapsedPathFindingCountdown > 0.0f)
		{
			myElapsedPathFindingCountdown -= aDeltaTime.GetSeconds();

			return;
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

		CU::Vector2f velocity = (myVelocity * SCALAR) * aDeltaTime.GetSeconds();

		if(myIsPlayerAlive == true)
		{
			if (PollingStation::playerObject != nullptr)
			{
				if (CU::Vector3f(GetParent()->GetWorldPosition() - PollingStation::playerObject->GetWorldPosition()).Length2() < 2000.0f * 2000.0f)
				{
					SComponentMessageData data;
					data.myGameObject = PollingStation::playerObject;
					GetParent()->NotifyComponents(eComponentMessageType::eSetSkillTargetObject, data);
				}
			}
		
		}




		if (myTrianglePointer != nullptr)
		{
			myPath.RemoveAll();

			CU::Vector3f newPosition = GetParent()->GetWorldPosition() + CU::Vector3f(velocity.x, 0.0f, velocity.y);
			CPath::SWaypoint wayPoint;
			wayPoint.myPosition = newPosition;
			myPath.Add(wayPoint);

			eComponentMessageType type = eComponentMessageType::eSetPathPosition;
			SComponentMessageData data;
			data.myPathPointer = &myPath;
			GetParent()->NotifyComponents(type, data);
			GetParent()->NotifyComponents(eComponentMessageType::eMoving, SComponentMessageData());
		}
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
	case (eComponentMessageType::eEnterNewTriangle):
		myTrianglePointer = aMessageData.myTrianglePointer;
		break;
	case(eComponentMessageType::eNotOnNavmesh):
		CreatePathToPlayer();
		break;
	case(eComponentMessageType::eDied):
		myIsActive = false;
		break;
	case eComponentMessageType::eRespawned:
		myIsActive = true;
		break;
	case(eComponentMessageType::eEnemyReturnToSpawnPoint):
	{
		myIsPlayerAlive = false;
	}
	break;
	case(eComponentMessageType::eEnemyStartChaseAgain):
	{
		myIsPlayerAlive = true;
	}
	break;
	default:
		break;
	}
}

void CAIControllerComponent::CreatePathToPlayer()
{
	CNavmesh* navmesh = PollingStation::Navmesh;
	CU::Vector3f intersectingPoint;

	SNavmeshNode startNode, endNode;
	startNode.myPosition = GetParent()->GetWorldPosition();
	startNode.myTriangle = myTrianglePointer;
	DL_PRINT("Calc Path");

	endNode.myPosition = PollingStation::playerObject->GetWorldPosition();
	endNode.myTriangle = &navmesh->GetClosestTriangle(endNode.myPosition);

	myPath = navmesh->CalculatePath(startNode, endNode);
	myPath.Smooth();

	eComponentMessageType type = eComponentMessageType::eSetPath;
	SComponentMessageData data;
	data.myPathPointer = &myPath;
	GetParent()->NotifyComponents(type, data);
	GetParent()->NotifyComponents(eComponentMessageType::eMoving, SComponentMessageData());

	myElapsedPathFindingCountdown = 2.0f;
}
