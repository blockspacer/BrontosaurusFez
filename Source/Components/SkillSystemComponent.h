#pragma once
#include "../CommonUtilities/GrowingArray.h"
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
	void AddSkill();
private:
	CU::GrowingArray<Skill*> mySkills;
	CGameObject* myTarget;
};

