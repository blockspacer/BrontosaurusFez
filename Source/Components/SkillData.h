#pragma once
#include "HealthPoint.h"
struct SkillData
{
	enum class SkillName
	{
		BasicAttack,
		SweepAttack,
		WhirlWind
	};

	SkillName skillName;
	float range;
	float coolDown;
	float activationRadius;
	float animationDuration;
	float manaCost;
	HealthPoint damage;
	bool isAOE;
	bool isChannel;
};