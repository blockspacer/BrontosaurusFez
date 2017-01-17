#pragma once
#include "HealthPoint.h"
struct SkillData
{

	std::string skillName;
	float range;
	float coolDown;
	float activationRadius;
	float animationDuration;
	float manaCost;
	HealthPoint damage;
	bool isAOE;
	bool isChannel;
};