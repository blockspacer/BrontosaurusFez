#pragma once
#include "HealthPoint.h"
#include "ManaPoint.h"
namespace Stats
{
	typedef int StatPoint;
	struct SBaseStats
	{

		float DamageModifier = 1.0f;
		float HealthDropChance = 1.0f;
		float ManaDropChance = 1.0f;
		float GoldGetModifier = 1.0f;
		float ManaCostModifier = 1.0f;
		
		SBaseStats operator+=(const SBaseStats& aStat)
		{
			DamageModifier += aStat.DamageModifier;
			HealthDropChance += aStat.HealthDropChance;
			ManaDropChance += aStat.ManaDropChance;
			GoldGetModifier += aStat.GoldGetModifier;
			ManaCostModifier += aStat.ManaCostModifier;

			return *this;
		}
	};
	struct SBonusStats
	{
		HealthPoint BonusHealth = 0;
		ManaPoint BonusMana = 0;

		float BonusMovementSpeed = 0.0f;
		float BonusDamageModifier = 0.0f;
		float BonusHealthDropChance = 0.0f;
		float BonusManaDropChance = 0.0f;
		float BonusGoldGetModifier = 0.0f;
		float BonusManaCostModifier = 0.0f;

		SBonusStats operator+=(const SBonusStats& aStats)
		{
			BonusMovementSpeed += aStats.BonusMovementSpeed;
			BonusHealth += aStats.BonusHealth;
			BonusMana += aStats.BonusMana;
			BonusDamageModifier = aStats.BonusDamageModifier;
			BonusGoldGetModifier = aStats.BonusGoldGetModifier;
			BonusHealthDropChance = aStats.BonusHealthDropChance;
			BonusManaDropChance = aStats.BonusManaDropChance;
			BonusManaCostModifier = aStats.BonusManaCostModifier;
			return *this;
		}
	};
	struct STotalStats
	{
		HealthPoint MaxHealth;
		ManaPoint MaxMana;

		float MaxMovementSpeed;
		float MaxDamageModifier;
		float MaxHealthDropChance;
		float MaxManaDropChance;
		float MaxGoldGetModifier;
		float MaxManaConstModifier;
	};
}