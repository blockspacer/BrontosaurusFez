#pragma once
#include "Component.h"
class SkillComponent : public CComponent
{
public:
	SkillComponent();
	~SkillComponent();
	void Update(float aDeltaTime);
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;
private:
	unsigned short myDamage;
	float myElapsedTime;
	bool myIsAOE;

	bool myIsActive;
};

