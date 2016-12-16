#pragma once

namespace Stats
{
	typedef int StatPoint;
	struct SBaseStatbody
	{
		StatPoint Strength;
		StatPoint Dexterity;
		StatPoint Intelligence;
		StatPoint Vitality;
	};
	struct SBonusStats
	{
		float BonusCritChance;
		float BonusCritDamage;
		float BonusMovementSpeed;
		int BonusArmor;
		int BonusDamage;
		int BonusHealth;
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