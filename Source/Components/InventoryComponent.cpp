#include "stdafx.h"
#include "InventoryComponent.h"
#include "..\PostMaster\PostMaster.h"

CInventoryComponent::CInventoryComponent()
{
	myHats.Init(16);
	myType = eComponentType::eInventory;

}

CInventoryComponent::~CInventoryComponent()
{
}

void CInventoryComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	switch (aMessageType)
	{
		case eComponentMessageType::eLootPickedUp:
			AddGold(aMessageData.myUShort);
			break;
	}



	aMessageData;
	aMessageType;
}

void CInventoryComponent::Destroy()
{
}

void CInventoryComponent::AddHat(SHat& aHat)
{
	myHats.Add(aHat);
}

void CInventoryComponent::AddGold(unsigned int aAmountOfGold)
{
	myGold += aAmountOfGold;
}