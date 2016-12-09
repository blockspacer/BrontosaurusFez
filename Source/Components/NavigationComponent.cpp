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
		CalculatePath(aMessageData.myVector2);
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
	myPath.Add(aTargetPosition);


	eComponentMessageType type = eComponentMessageType::eSetPath;
	SComponentMessageData data;
	data.myVector2ListPointer = &myPath;
	GetParent()->NotifyComponents(type, data);
}
