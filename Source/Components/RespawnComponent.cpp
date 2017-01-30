#include "stdafx.h"
#include "RespawnComponent.h"
#include "../Game/PollingStation.h"
#include "PlayerData.h"



RespawnComponent::RespawnComponent()
{
	myRespawnTime = 1;
	myElapsedTime = 0;
	myType = eComponentType::eRespawn;
}


RespawnComponent::~RespawnComponent()
{
}

void RespawnComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	switch (aMessageType)
	{
	case eComponentMessageType::eDied:
		
		PollingStation::playerData->myGold -= PollingStation::playerData->myGold * myGoldLossPercentage;
		myHasDied = true;

		break;
	}

}

void RespawnComponent::Destroy()
{
}

void RespawnComponent::SetGoldLossPercentage(const float aPercentage)
{
	myGoldLossPercentage = aPercentage;
}

void RespawnComponent::SetRespawnTime(const float aNumberOfSeconds)
{
	myRespawnTime = aNumberOfSeconds;
}

void RespawnComponent::Update(const float aDeltaTime)
{
	if (myHasDied == true)
	{
		myElapsedTime += aDeltaTime;
		if (myElapsedTime >= myRespawnTime)
		{
			GetParent()->NotifyComponents(eComponentMessageType::eRespawned, SComponentMessageData());
			myHasDied = false;
			myElapsedTime = 0;
		}
	}
}
