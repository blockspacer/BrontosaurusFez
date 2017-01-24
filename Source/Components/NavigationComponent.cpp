#include "stdafx.h"
#include "NavigationComponent.h"
#include "GameObject.h"


#include "PollingStation.h"

NavigationComponent::NavigationComponent()
{
	myPath.myWaypoints.Init(64);
	myType = eComponentType::eNavigation;
}


NavigationComponent::~NavigationComponent()
{
}

void NavigationComponent::Update(float aDeltaTime)
{
}

void NavigationComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	switch (aMessageType)
	{
	case eComponentMessageType::eSetNavigationTarget:
		CalculatePath(aMessageData.myVector2f);
		break;
	default:
		break;
	}
}

void NavigationComponent::Destroy()
{
}

void NavigationComponent::CalculatePath(const CU::Vector2f& aTargetPosition)
{
	myPath.myWaypoints.RemoveAll();
	
	//CNavmesh* navmesh = PollingStation::Navmesh;
	CNavmesh* navmesh = nullptr;
	if (navmesh == nullptr)
	{
		myPath.myWaypoints.Add(CU::Vector3f(aTargetPosition.x, 0.0f, aTargetPosition.y));
	}
	else
	{
		CNavmesh::SNavmeshNode startNode;
		CNavmesh::SNavmeshNode endNode;
		CU::Vector3f intersectionpoint;// = { aTargetPosition.x, 0.0f, aTargetPosition.y }; // this thing with y & z is confusing as hell
		float height;

		bool clickedOnNavmesh = navmesh->IsValid(aTargetPosition, endNode.myTriangle, intersectionpoint);
		if (clickedOnNavmesh)
		{
			endNode.myPosition = intersectionpoint;
		}
		else
		{
			endNode.myTriangle = &navmesh->GetClosestTriangle({aTargetPosition.x, 0.0f, aTargetPosition.y});
			endNode.myPosition = endNode.myTriangle->CenterPosition;
		}

		bool isStandingOnNM = navmesh->IsValid(GetParent()->GetWorldPosition(), startNode.myTriangle, intersectionpoint);
		if (isStandingOnNM)
		{
			startNode.myPosition = intersectionpoint;
		}
		else
		{
			startNode.myTriangle = &navmesh->GetClosestTriangle(GetParent()->GetWorldPosition());
			startNode.myPosition = startNode.myTriangle->CenterPosition;
		}

		myPath = navmesh->CalculatePath(startNode, endNode);

	}

	eComponentMessageType type = eComponentMessageType::eSetPath;
	SComponentMessageData data;
	data.myVector3ListPointer = &myPath.myWaypoints;
	GetParent()->NotifyComponents(type, data);
}
