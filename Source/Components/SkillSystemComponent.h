#pragma once
#include "../CommonUtilities/GrowingArray.h"
class Skill;
class SkillSystemComponent
{
public:
	SkillSystemComponent();
	~SkillSystemComponent();

private:
	CU::GrowingArray<Skill*> mySkills;
};

