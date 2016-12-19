#include "stdafx.h"
#include "SkillFactory.h"
#include "Skill.h"

SkillFactory::SkillFactory()
{
	mySkills.Init(500);
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
	SAFE_DELETE(ourInstance);
}

Skill * SkillFactory::CreateSkill(const char * aSkillName)
{
	Skill* newSkill = new Skill;
	mySkills.Add(newSkill);
	return newSkill;
}
