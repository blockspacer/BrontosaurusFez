#include "stdafx.h"
#include "DropComponent.h"
#include "../Game/PollingStation.h"
#include "PlayerData.h"

DropComponent::DropComponent(const int aAmountOfGoldToDrop)
{
	myGoldToDrop = aAmountOfGoldToDrop;
	myHealthGlobeDropRate = 100;
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
		PollingStation::playerData->myGold += myGoldToDrop;
	}
}

void DropComponent::Destroy()
{
}

bool DropComponent::CalculateDropHealthGlobe()
{
	int result = (rand() % 100) +1;

	if (result <= myHealthGlobeDropRate)
	{
		return true;
	}


	return false;
}
