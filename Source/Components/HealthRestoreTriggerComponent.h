#pragma once
#include "Component.h"
class HealthRestoreTriggerComponent : public CComponent
{
public:
	HealthRestoreTriggerComponent();
	~HealthRestoreTriggerComponent();

	void Update(const float& aDeltaTime);

	void SetHealAmount(const unsigned short& aAmount);
	void SetCoolDown(const float& aTime);

	void Destroy() override;

	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;

private:
	unsigned short myHealAmount;
	float myCoolDown;
	float myElaspedTime;
	bool myIsOnCoolDown;
};

