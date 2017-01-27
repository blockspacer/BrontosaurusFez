#include "stdafx.h"
#include "BlessingTowerComponent.h"
#include "../Game/PollingStation.h"
#include "StatStructs.h"

BlessingTowerComponent::BlessingTowerComponent()
{
	myElapsedBuffTime = 0.0f;
	myIsActivated = false;
	myHaveBeenDeactivated = false;
	myDamageBonusStats = new Stats::SBonusStats;
}


BlessingTowerComponent::~BlessingTowerComponent()
{
}

void BlessingTowerComponent::Update(float aDeltaTime)
{
	if(myIsActivated == true)
	{
		myElapsedBuffTime += aDeltaTime;
		if(myElapsedBuffTime > myDuration)
		{
			if(myHaveBeenDeactivated == false)
			{
				myHaveBeenDeactivated = true;
				myDamageBonusStats->BonusDamageModifier *= -1;
				SComponentMessageData damaageData;

				damaageData.myStatsToAdd = myDamageBonusStats;
				PollingStation::playerObject->NotifyComponents(eComponentMessageType::eAddStats, damaageData);
			}
		}
	}
}

void BlessingTowerComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	if (aMessageType == eComponentMessageType::eActivate)
	{
		if (myIsActivated == false)
		{
			myIsActivated = true;
			
			SComponentMessageData manaData;
			manaData.myInt = myGiveManaAmount;
			PollingStation::playerObject->NotifyComponents(eComponentMessageType::eRestoreMana, manaData);

			SComponentMessageData damaageData;
			
			damaageData.myStatsToAdd = myDamageBonusStats;
			PollingStation::playerObject->NotifyComponents(eComponentMessageType::eAddStats, damaageData);
		}
	}
	
}

void BlessingTowerComponent::Destroy()
{
	SAFE_DELETE(myDamageBonusStats);
}
