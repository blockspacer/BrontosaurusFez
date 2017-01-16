#pragma once
#include "SkillData.h"

class Skill;
class SkillFactory
{
public:
	~SkillFactory();

	static SkillFactory& GetInstance();
	static void CreateInstance();
	static void DestroyInstance();

	Skill* CreateSkill(SkillData::SkillName aSkillName);
	void RegisterSkillData(SkillData* aSkillData);
	void RegisterSkills();
private:
	SkillFactory();
	
	CU::GrowingArray<Skill*> mySkills;
	CU::GrowingArray<SkillData*> mySkillDataList;
	static SkillFactory* ourInstance;
};

