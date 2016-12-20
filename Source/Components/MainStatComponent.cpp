#include "stdafx.h"
#include "MainStatComponent.h"
#include "HealthComponent.h"
#include "StatComponent.h"

CMainStatComponent::CMainStatComponent()
{
	myBaseStats = new Stats::SBaseStats;
	myBonusStats = new Stats::SBonusStats;
	myTotalStats = new Stats::STotalStats;

	myBaseStats->Strength = 0;
	myBaseStats->Dexterity = 0;
	myBaseStats->Intelligence = 0;
	myBaseStats->Vitality = 0;

	myBonusStats->BonusArmor = 0;
	myBonusStats->BonusCritChance = 0.0f;
	myBonusStats->BonusCritDamage = 0.0f;
	myBonusStats->BonusDamage = 0;
	myBonusStats->BonusMovementSpeed = 0.0f;
	myBonusStats->BonusHealth = 0;

	myType = eComponentType::eMainStat;
}

CMainStatComponent::~CMainStatComponent()
{
}

void CMainStatComponent::CalculateTotalStats()
{
	const float strengthScaling = 1 + (myBaseStats->Strength * 0.05f);
	const float dexterityScaling = 1 + (myBaseStats->Dexterity * 0.05f);
	const float intelligenceScaling = 1 + (myBaseStats->Intelligence * 0.05f);
	const float vitalityScaling = 1 + (myBaseStats->Vitality * 0.05f);

	const float dodgeDexScaling = myBaseStats->Dexterity * 0.01f;
	const float attackspeedDexScaling = myBaseStats->Dexterity * 0.01f;

	myTotalStats->BlockChance = 0.15f;
	myTotalStats->MaxHealth = 50 * vitalityScaling + myBonusStats->BonusHealth;
	myTotalStats->Armor = myBonusStats->BonusArmor * strengthScaling;
	myTotalStats->DamageReduction = myTotalStats->Armor * 0.01f;
	myTotalStats->DodgeChance = 0 + dodgeDexScaling;

	if (myTotalStats->DamageReduction > 0.8f)
	{
		myTotalStats->DamageReduction = 0.8f;
	}


	myTotalStats->AttackSpeed = 1 - attackspeedDexScaling;
	myTotalStats->CritMultiplier = 2 + myBonusStats->BonusCritDamage;
	myTotalStats->CritChance = 0.05 * dexterityScaling + myBonusStats->BonusCritChance;


	SComponentMessageData data;
	//data.myTotalStats = *myTotalStats;
	//GetParent()->NotifyComponents(eComponentMessageType::eStatsUpdated,SComponentMessageData());
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
