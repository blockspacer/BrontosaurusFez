#pragma once
#include "Component.h"
#include "HealthPoint.h"

class CGameObject;
struct SkillData;
class SkillComponent : public CComponent
{
public:
	SkillComponent(CGameObject* aUserObject, SkillData* aSkillDataPointer);
	~SkillComponent();
	void Update(float aDeltaTime);
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;
	void DoDamage(CGameObject* aGameObjectToDamage);
private:
	CGameObject* myUserObject;
	SkillData* mySkillData;
	float myElapsedTime;

	bool myIsActive;
};


