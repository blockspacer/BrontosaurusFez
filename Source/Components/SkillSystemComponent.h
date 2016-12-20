#pragma once

#include "Component.h"
class Skill;
class SkillSystemComponent : public CComponent
{
public:
	SkillSystemComponent();
	~SkillSystemComponent();
	void Update(float aDeltaTime);
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;
	void AddSkill(const char* aSkillName);
private:
	CU::GrowingArray<Skill*> mySkills;
	CU::Vector3f myTargetPosition;
};

