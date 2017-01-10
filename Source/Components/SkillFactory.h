#pragma once
class Skill;
struct SkillData;
class SkillFactory
{
public:
	~SkillFactory();

	static SkillFactory& GetInstance();
	static void CreateInstance();
	static void DestroyInstance();

	Skill* CreateSkill(const char* aSkillName);
	void RegisterSkillData(SkillData* aSkillData);
private:
	SkillFactory();
	
	CU::GrowingArray<Skill*> mySkills;
	CU::GrowingArray<SkillData*> mySkillDataList;
	static SkillFactory* ourInstance;
};

