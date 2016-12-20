#pragma once

namespace Stats
{
	typedef int StatPoint;
	struct SBaseStats
	{
		StatPoint Strength = 0;
		StatPoint Dexterity = 0;
		StatPoint Intelligence = 0;
		StatPoint Vitality = 0;
		SBaseStats operator+=(const SBaseStats& aStat)
		{
			Strength += aStat.Strength;
			Dexterity += aStat.Dexterity;
			Intelligence += aStat.Intelligence;
			Vitality += aStat.Vitality;

			return *this;
		}
	};
	struct SBonusStats
	{
		float BonusCritChance = 0.f;
		float BonusCritDamage = 0.f;
		float BonusMovementSpeed = 0.f;
		int BonusArmor = 0;
		int BonusDamage = 0;
		int BonusHealth = 0;
		SBonusStats operator+=(const SBonusStats& aStats)
		{
			BonusCritChance += aStats.BonusCritChance;
			BonusCritDamage += aStats.BonusCritDamage;
			BonusMovementSpeed += aStats.BonusMovementSpeed;
			BonusArmor += aStats.BonusArmor;
			BonusDamage += aStats.BonusDamage;
			BonusHealth += aStats.BonusHealth;

			return *this;
		}
	};
	struct STotalStats
	{
		float CritChance;
		float CritMultiplier;
		float BlockChance;
		float DodgeChance;
		float DamageReduction;
		float AttackSpeed;
		int Armor;
		int Damage;
		int MaxHealth;
	};
}