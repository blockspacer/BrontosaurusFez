#include "stdafx.h"
#include "RespawnComponent.h"
#include "../Game/PollingStation.h"
#include "PlayerData.h"
#include "../PostMaster/PostMaster.h"
#include "../PostMaster/GameEventMessageEvent.h"
#include "../CommonUtilities/DynamicString.h"



RespawnComponent::RespawnComponent()
{
	myRespawnTime = 2.5f;
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
		int gold = PollingStation::playerData->GetGold();
		int goldAfter = PollingStation::playerData->RemoveGold(static_cast<unsigned short>(static_cast<float>(PollingStation::playerData->GetGold()) * (myGoldLossPercentage * 0.01f)));

		myHasDied = true;

		std::string temp("You lost ");
		temp += std::to_string(gold - goldAfter);
		temp += " gold.";

		CU::DynamicString temp2(temp.c_str());


		PostMaster::GetInstance().SendLetter(eMessageType::eGameEventMessage, CGameEventMessageEvent({ "You Died, you lost some of your fortune." ,  temp2 }));

		for(unsigned short i=0; i < PollingStation::myThingsEnemiesShouldAvoid.Size(); i++)
		{
			PollingStation::myThingsEnemiesShouldAvoid[i]->NotifyOnlyComponents(eComponentMessageType::eEnemyReturnToSpawnPoint, SComponentMessageData());
		}
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
			for (unsigned short i = 0; i < PollingStation::myThingsEnemiesShouldAvoid.Size(); i++)
			{
				PollingStation::myThingsEnemiesShouldAvoid[i]->NotifyOnlyComponents(eComponentMessageType::eEnemyStartChaseAgain, SComponentMessageData());
			}
		}
	}
}
