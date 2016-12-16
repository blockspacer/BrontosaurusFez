#include "stdafx.h"
#include "StatComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"

CStatComponent::CStatComponent()
{
	myBaseStats = new Stats::SBaseStatbody;
	myBonusStats = new Stats::SBonusStats;
	myTotalStats = new Stats::STotalStats;
	myLevelUpBonus = new Stats::SBaseStatbody;

	myLevelUpBonus->Strength = 5;
	myLevelUpBonus->Dexterity = 3;
	myLevelUpBonus->Intelligence = 2;
	myLevelUpBonus->Vitality = 4;

	myBaseStats->Strength = myLevelUpBonus->Strength;
	myBaseStats->Dexterity = myLevelUpBonus->Dexterity;
	myBaseStats->Intelligence = myLevelUpBonus->Intelligence;
	myBaseStats->Vitality = myLevelUpBonus->Vitality;

	myBonusStats->BonusArmor = 0;
	myBonusStats->BonusCritChance = 0.0f;
	myBonusStats->BonusCritDamage = 0.0f;
	myBonusStats->BonusDamage = 0;
	myBonusStats->BonusMovementSpeed = 0.0f;
	myBonusStats->BonusHealth = 0;

	CalculateTotalStats();
}


CStatComponent::~CStatComponent()
{
}

void CStatComponent::Set(const int aStrength, const int aDexterity, const int aIntelligence, const int aVitality)
{
	myBaseStats->Strength = aStrength;
	myBaseStats->Dexterity = aDexterity;
	myBaseStats->Vitality = aVitality;
	myBaseStats->Intelligence = aIntelligence;
}

void CStatComponent::LevelUp()
{
	myBaseStats->Dexterity += myLevelUpBonus->Dexterity;
	myBaseStats->Intelligence += myLevelUpBonus->Intelligence;
	myBaseStats->Strength += myLevelUpBonus->Strength;
	myBaseStats->Vitality += myLevelUpBonus->Vitality;
	CalculateTotalStats();
}

void CStatComponent::CalculateTotalStats()
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

void CStatComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	switch (aMessageType)
	{
	case eComponentMessageType::eSetMaxHealthFromStats:
		static_cast<CHealthComponent*>(aMessageData.myComponent)->SetMaxHealth(myTotalStats->MaxHealth);
		break;
	default:
		break;
	}
}

void CStatComponent::Destroy()
{
	delete myBonusStats;
	myBonusStats = nullptr;
	delete myBaseStats;
	myBaseStats = nullptr;
	delete myTotalStats;
	myTotalStats = nullptr;
}
