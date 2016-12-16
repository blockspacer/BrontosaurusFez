#include "stdafx.h"
#include "InventoryComponent.h"
#include <Hat.h>


CInventoryComponent::CInventoryComponent()
{
	myHats.Init(16);
}

CInventoryComponent::~CInventoryComponent()
{
}

void CInventoryComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	aMessageData;
	aMessageType;
}

void CInventoryComponent::Destroy()
{
}

void CInventoryComponent::AddHat(CHat& aHat)
{
	myHats.Add(aHat);
}
