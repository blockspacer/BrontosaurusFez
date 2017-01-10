#pragma once
#include "HealthPoint.h"
struct SkillData
{
	const char* skillName;
	float range;
	float coolDown;
	float activationRadius;
	float animationDuration;
	HealthPoint damage;
	bool isAOE;
};