#pragma once

#include "Component.h"
#include "SkillData.h"
#include "../CommonUtilities/vector3.h"
class Skill;
class SkillSystemComponent : public CComponent
{
public:
	SkillSystemComponent();
	~SkillSystemComponent();
	void Update(float aDeltaTime);
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;
	void AddSkill(SkillData::SkillName aSkillName);
private:
	CU::GrowingArray<Skill*> mySkills;
	CU::Vector3f myTargetPosition;
	bool myIsActive;
};

