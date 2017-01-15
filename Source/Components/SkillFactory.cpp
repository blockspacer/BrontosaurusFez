#include "stdafx.h"
#include "SkillFactory.h"
#include "Skill.h"

#include <iostream>

#include "../CommonUtilities/PJWrapper.h"


SkillFactory* SkillFactory::ourInstance = nullptr;

SkillFactory::SkillFactory()
{
	mySkills.Init(500);
	mySkillDataList.Init(25);
}


SkillFactory::~SkillFactory()
{
}

SkillFactory & SkillFactory::GetInstance()
{
	assert(ourInstance != nullptr && "skill Factory not created (is NULL)");
	return *ourInstance;
}

void SkillFactory::CreateInstance()
{
	assert(ourInstance == nullptr && "input controller component manager already created");
	ourInstance = new SkillFactory();
}

void SkillFactory::DestroyInstance()
{
	assert(ourInstance != nullptr && "input controller component manager not created (is NULL)");
	ourInstance->mySkills.DeleteAll();
	ourInstance->mySkills.Destroy();
	ourInstance->mySkillDataList.DeleteAll();
	ourInstance->mySkillDataList.Destroy();
	SAFE_DELETE(ourInstance);
}

Skill * SkillFactory::CreateSkill(SkillData::SkillName aSkillName)
{
	for(unsigned short i=0; i < mySkillDataList.Size(); i++)
	{
		if(mySkillDataList[i]->skillName == aSkillName)
		{
			Skill* newSkill = new Skill(mySkillDataList[i]);
			mySkills.Add(newSkill);
			return mySkills.GetLast();
		}
	}

	DL_ASSERT("Skill Factory couldn't find a SkillData with that name. Check spelling and/or yell at Marcus and maybe Alex.");
	return nullptr;
	/*Skill* newSkill = new Skill;
	if(aSkillName == "BasicAttack")
	{
	
		mySkills.Add(newSkill);
	}
	else
	{
		std::cout << "Skill not yet implemented" << std::endl;
	}
	return newSkill;*/
}

void SkillFactory::RegisterSkillData(SkillData * aSkillData)
{
	mySkillDataList.Add(aSkillData);
}

void SkillFactory::RegisterSkills()
{
	std::string error;
	CU::CPJWrapper json;
	json.Parse("");

	if (json.count("name") < 1)
	{
		DL_PRINT("Json file is missing");
	}

	SkillData skill;
	skill.activationRadius = json.at("activationRadius").GetFloat();
	skill.animationDuration = json.at("animationDuration").GetFloat();
	skill.coolDown = json.at("coolDown").GetFloat();
	skill.damage = json.at("damage").GetUInt();
	skill.isAOE = json.at("isAOE").GetBool();
	skill.isChannel = json.at("isChannel").GetBool();
	skill.manaCost = json.at("manaCost").GetUInt();
	skill.range = json.at("range").GetFloat();
	
	std::string name = json.at("name").GetString();

	if (name == "WhirlWind")
	{
		skill.skillName = SkillData::SkillName::WhirlWind;
	}
	else if (name == "BasicAttack")
	{
		skill.skillName = SkillData::SkillName::BasicAttack;
	}
	else if (name == "SweepAttack")
	{
		skill.skillName = SkillData::SkillName::SweepAttack;
	}
	else
	{
		DL_PRINT("Couldn't find match for skill name in json file");
	}
}
