#pragma once
#include "Component.h"

class CGameObject;
class SkillComponent : public CComponent
{
public:
	SkillComponent(CGameObject* aUserObject);
	~SkillComponent();
	void Update(float aDeltaTime);
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;
	void DoDamage(CGameObject* aGameObjectToDamage);
private:
	CGameObject* myUserObject;
	unsigned short myDamage;
	float myElapsedTime;
	bool myIsAOE;

	bool myIsActive;
};


