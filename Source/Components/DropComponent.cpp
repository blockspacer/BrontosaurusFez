#include "stdafx.h"
#include "DropComponent.h"
#include "../Game/PollingStation.h"
#include "PlayerData.h"

DropComponent::DropComponent(const int aAmountOfGoldToDrop)
{
	myGoldToDrop = aAmountOfGoldToDrop;
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
