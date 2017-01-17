#include "stdafx.h"
#include "SkillFactory.h"
#include "Skill.h"

#include <iostream>

#include "../CommonUtilities/PJWrapper.h"
#include "../CommonUtilities/JsonValue.h"


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

Skill * SkillFactory::CreateSkill(char* aSkillName)
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
	CU::CJsonValue SkillBluePrints;
	const std::string& errorString = SkillBluePrints.Parse("Json/Skills.json");
	CU::CJsonValue levelsArray = SkillBluePrints.at("skills");

	for (unsigned int i = 0; i < levelsArray.Size(); ++i)
	{
		SkillData* skill = new SkillData;
		skill->activationRadius = levelsArray[i].at("activationRadius").GetFloat();
		skill->animationDuration = levelsArray[i].at("animationDuration").GetFloat();
		skill->coolDown = levelsArray[i].at("coolDown").GetFloat();
		skill->damage = levelsArray[i].at("damage").GetUInt();
		skill->isAOE = levelsArray[i].at("isAOE").GetBool();
		skill->isChannel = levelsArray[i].at("isChannel").GetBool();
		skill->manaCost = levelsArray[i].at("manaCost").GetUInt();
		skill->range = levelsArray[i].at("range").GetFloat();
			 
		skill->skillName = levelsArray[i].at("name").GetString().c_str();

		mySkillDataList.Add(skill);
	}
}
