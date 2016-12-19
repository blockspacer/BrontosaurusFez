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

void CInventoryComponent::AddGold(unsigned int aAmountOfGold)
{
	myGold += aAmountOfGold;
}

void CInventoryComponent::RenderHatsInInventory()
{
	for (int i = 0; i < myHats.Size(); ++i)
	{
		myHats[i].Render(); // mebe
	}
}
