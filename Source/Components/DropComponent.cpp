#include "stdafx.h"
#include "DropComponent.h"
#include "../Game/PollingStation.h"
#include "PlayerData.h"
#include "../Game/PickupFactory.h"
#include "../Game/MasterAI.h"



DropComponent::DropComponent(const int aAmountOfGoldToDrop, const int aHealthDropPercentChance, const int aManaDropPercentChance, const char* aHatToDrop)
{
	myGoldToDrop = aAmountOfGoldToDrop;
	myHealthGlobeDropRate = aHealthDropPercentChance;
	myManaGlobeDropRate = aManaDropPercentChance;
	myHatToDrop = aHatToDrop;
	myType = eComponentType::eDrop;
}


DropComponent::~DropComponent()
{
}

void DropComponent::Update(float aDeltaTime)
{
}

void DropComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	if (aMessageType == eComponentMessageType::eDied)
	{
		CPickupFactory::GetInstance().CreateGoldPickup(GetParent()->GetWorldPosition(),myGoldToDrop * PollingStation::playerData->myGoldGetModifier);

		SComponentMessageData data;
		data.myString = "GoldDrop";
		GetParent()->NotifyComponents(eComponentMessageType::ePlaySound, data);

		if (CalculateDropHealthGlobe() == true)
		{
			CPickupFactory::GetInstance().CreateHealthGlobe(GetParent()->GetWorldPosition());
		}

		if (CalculateDropManaGlobe() == true)
		{
			CPickupFactory::GetInstance().CreateManaGlobe(GetParent()->GetWorldPosition());
		}

		if (myHatToDrop != "")
		{
			CPickupFactory::GetInstance().CreateHatDrop(GetParent()->GetWorldPosition(), myHatToDrop.c_str());
		}
	}
}

void DropComponent::Destroy()
{
}

bool DropComponent::CalculateDropHealthGlobe()
{
	int result = (rand() % 100) +1;

	float MasterAImodifier = CMasterAI::GetInstance().DetermineHealthDrop();
	
	//std::cout << "Modified Health drop chance: " << MasterAImodifier << std::endl;

	if (result <= myHealthGlobeDropRate + MasterAImodifier)
	{
		return true;
	}


	return false;
}

bool DropComponent::CalculateDropManaGlobe()
{
	int result = (rand() % 100) + 1;

	if (result <= myManaGlobeDropRate)
	{
		return true;
	}

	return false;
}
