#include "stdafx.h"
#include "MainStatComponent.h"
#include "HealthComponent.h"
#include "StatComponent.h"

CMainStatComponent::CMainStatComponent()
{
	myBaseStats = new Stats::SBaseStats;
	myBonusStats = new Stats::SBonusStats;
	myTotalStats = new Stats::STotalStats;

	myBaseStats->DamageModifier = 1;
	myBaseStats->GoldGetModifier = 1;
	myBaseStats->HealthDropChance = 0;
	myBaseStats->ManaDropChance = 0;
	myBaseStats->ManaConstModifier = 1;

	myBonusStats->BonusDamageModifier = 0.0f;
	myBonusStats->BonusGoldGetModifier = 0.0f;
	myBonusStats->BonusHealth = 0;
	myBonusStats->BonusHealthDropChance = 0.0f;
	myBonusStats->BonusMovementSpeed = 0.0f;
	myBonusStats->BonusMana = 0;
	myBonusStats->BonusManaDropChance = 0.0f;
	myBonusStats->BonusManaConstModifier = 0.0f;

	myType = eComponentType::eMainStat;
}

CMainStatComponent::~CMainStatComponent()
{
}

void CMainStatComponent::CalculateTotalStats()
{


	myTotalStats->MaxDamageModifier = myBaseStats->DamageModifier + myBonusStats->BonusDamageModifier;
	myTotalStats->MaxGoldGetModifier = myBaseStats->GoldGetModifier + myBonusStats->BonusGoldGetModifier;
	myTotalStats->MaxHealthDropChance = myBaseStats->HealthDropChance + myBonusStats->BonusHealthDropChance;
	myTotalStats->MaxManaDropChance = myBaseStats->ManaDropChance + myBonusStats->BonusManaDropChance;
	myTotalStats->MaxManaConstModifier = myBaseStats->ManaConstModifier + myBonusStats->BonusManaConstModifier;
	
	//Add message for speed health and mana

	SComponentMessageData data;
	data.myStatStruct = *myTotalStats;
	GetParent()->NotifyComponents(eComponentMessageType::eStatsUpdated,SComponentMessageData());
}

void CMainStatComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	switch (aMessageType)
	{
	case eComponentMessageType::eSetMaxHealthFromStats:
		static_cast<CHealthComponent*>(aMessageData.myComponent)->SetMaxHealth(myTotalStats->MaxHealth);
		break;
	case eComponentMessageType::eCollectStats:
		CStatComponent* collectedStats = static_cast<CStatComponent*>(aMessageData.myComponent);
		*myBaseStats += *collectedStats->myBaseStats;
		*myBonusStats += *collectedStats->myBonusStats;
		CalculateTotalStats();
		break;
	}
}

void CMainStatComponent::Destroy()
{
	SAFE_DELETE(myBaseStats);
	SAFE_DELETE(myBonusStats);
	SAFE_DELETE(myTotalStats);
}
