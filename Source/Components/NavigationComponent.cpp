#include "stdafx.h"
#include "NavigationComponent.h"
#include "GameObject.h"

NavigationComponent::NavigationComponent()
{
	myPath.Init(100);
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

void NavigationComponent::CalculatePath(CU::Vector2f aTargetPosition)
{
	myPath.RemoveAll();
	
	//Need to add real calculation here.
	myPath.Add(CU::Vector3f(aTargetPosition.x, 0.0f, aTargetPosition.y));


	eComponentMessageType type = eComponentMessageType::eSetPath;
	SComponentMessageData data;
	data.myVector3ListPointer = &myPath;
	GetParent()->NotifyComponents(type, data);
}
