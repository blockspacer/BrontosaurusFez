#pragma once
class Skill;
class SkillFactory
{
public:
	~SkillFactory();

	static SkillFactory& GetInstance();
	static void CreateInstance();
	static void DestroyInstance();

	Skill* CreateSkill(const char* aSkillName);
private:
	SkillFactory();
	
	CU::GrowingArray<Skill*> mySkills;
	static SkillFactory* ourInstance;
};

