#include "stdafx.h"
#include "NavigationComponent.h"
#include "GameObject.h"
#include "ThreadPool.h"

#include "PollingStation.h"

NavigationComponent::NavigationComponent()
{
	myType = eComponentType::eNavigation;
	myThreadHasFinished = false;
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
	myPath.RemoveAll();
	
	CNavmesh* navmesh = PollingStation::Navmesh;
	//CNavmesh* navmesh = nullptr;
	if (navmesh == nullptr)
	{
		CPath::SWaypoint wayPoint;
		wayPoint.myPosition = CU::Vector3f(aTargetPosition.x, 0.0f, aTargetPosition.y);
		myPath.Add(wayPoint);// this thing with y & z is confusing as hell
	}
	else
	{
		myTargetPosition = aTargetPosition;
		PathfindOnNavmesh();
	}

	eComponentMessageType type = eComponentMessageType::eSetPath;
	SComponentMessageData data;
	data.myPathPointer = &myPath;
	GetParent()->NotifyComponents(type, data);
}

void NavigationComponent::PathfindOnNavmesh()
{
	CNavmesh* navmesh = PollingStation::Navmesh;

	SNavmeshNode startNode;
	SNavmeshNode endNode;
	CU::Vector3f intersectionpoint;
	float height;

	bool clickedOnNavmesh = navmesh->IsValid(myTargetPosition, endNode.myTriangle, intersectionpoint);
	if (clickedOnNavmesh)
	{
		endNode.myPosition = intersectionpoint;
	}
	else
	{
		endNode.myTriangle = &navmesh->GetClosestTriangle({ myTargetPosition.x, 0.0f, myTargetPosition.y });
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
	myPath.Smooth();
}

