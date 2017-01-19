#include "stdafx.h"
#include "DropComponent.h"
#include "../Game/PollingStation.h"
#include "PlayerData.h"
#include "../Game/PickupFactory.h"
#include "../Game/MasterAI.h"



DropComponent::DropComponent(const int aAmountOfGoldToDrop, const int aDropPercentChance)
{
	myGoldToDrop = aAmountOfGoldToDrop;
	myHealthGlobeDropRate = aDropPercentChance;
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
		//PollingStation::playerData->myGold += myGoldToDrop * PollingStation::playerData->myGoldGetModifier;
		CPickupFactory::GetInstance().CreateGoldPickup(GetParent()->GetWorldPosition(),myGoldToDrop * PollingStation::playerData->myGoldGetModifier);
		if (CalculateDropHealthGlobe() == true)
		{
			CPickupFactory::GetInstance().CreateHealthGlobe(GetParent()->GetWorldPosition());
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
