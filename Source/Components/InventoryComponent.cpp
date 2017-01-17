#include "stdafx.h"
#include "InventoryComponent.h"
#include "..\PostMaster\PostMaster.h"
#include "HatActivator.h"
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
		case eComponentMessageType::eAddHat:
			AddHat(*aMessageData.myHat);
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
	if(myHats.GetLast().hatActivator->GetIsActive() == true)
	{
		SComponentMessageData data;
		data.myStatsToAdd = myHats.GetLast().stat;
		GetParent()->NotifyComponents(eComponentMessageType::eAddStats, data);
	}
}

void CInventoryComponent::AddGold(unsigned int aAmountOfGold)
{
	myGold += aAmountOfGold;
}