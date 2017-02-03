#pragma once
#include "HealthPoint.h"
struct SkillData
{

	std::string skillName;
	std::string animationPlayedName;
	float range;
	float coolDown;
	float activationRadius;
	float animationDuration;
	float animationWindDown;
	float manaCost;
	HealthPoint damage;
	bool isAOE;
	bool isChannel;
	float damageModifier;
	float manaCostModifier;
	float damageBonus;
	float manaRefund;
	float movementSpeedBuffModifier;
	unsigned char numberOfEnemiesToSpawn;
};