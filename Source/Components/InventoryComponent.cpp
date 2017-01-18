#include "stdafx.h"
#include "InventoryComponent.h"
#include "..\PostMaster\PostMaster.h"
#include "HatActivator.h"
#include "../Game/PollingStation.h"
#include "PlayerData.h"
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
		case eComponentMessageType::ePercentHPLeft:
			for(unsigned short i=0; i < myHats.Size(); i++)
			{
				myHats[i].hatActivator->CheckHealthLimit(aMessageData.myUChar / 100.0f);
			}
			break;
		case eComponentMessageType::ePercentMPLeft:
			for (unsigned short i = 0; i < myHats.Size(); i++)
			{
				myHats[i].hatActivator->CheckManaLimit(aMessageData.myUChar / 100.0f);
			}
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
		SComponentMessageData skillNameData;
		skillNameData.myString = myHats.GetLast().skillname;
		data.myStatsToAdd = myHats.GetLast().stat;
		GetParent()->NotifyComponents(eComponentMessageType::eAddStats, data);
		GetParent()->NotifyComponents(eComponentMessageType::eAddSkill, skillNameData);
		PollingStation::playerData->myGoldGetModifier += data.myStatsToAdd->BonusGoldGetModifier;
	}
}

void CInventoryComponent::AddGold(unsigned int aAmountOfGold)
{
	myGold += aAmountOfGold;
}