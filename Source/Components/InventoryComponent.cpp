#include "stdafx.h"
#include "InventoryComponent.h"


InventoryComponent::InventoryComponent()
{
	//Hats.Init(16);
}

InventoryComponent::~InventoryComponent()
{
}

void InventoryComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	aMessageData;
	aMessageType;
}

void InventoryComponent::Destroy()
{
}

void InventoryComponent::AddHat(const Hat& aHat)
{
	aHat;
	//myHats.Add(aHat);
}
