#include "stdafx.h"
#include "SkillFactory.h"
#include "Skill.h"

#include <iostream>

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
