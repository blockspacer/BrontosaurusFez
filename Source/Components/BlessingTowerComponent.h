#pragma once
#include "Component.h"
#include "ManaPoint.h"
namespace Stats 
{
	struct SBonusStats;
}
class BlessingTowerComponent : public CComponent
{
public:
	BlessingTowerComponent();
	~BlessingTowerComponent();

	void Update(float aDeltaTime);
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;

	inline void SetGiveManaAmount(ManaPoint aManaAmount);
	inline void SetDamageModifier(float aDamageModifier);
	inline void SetDuration(float aDuration);
private:
	Stats::SBonusStats* myDamageBonusStats;
	ManaPoint myGiveManaAmount;
	float myDuration;
	float myElapsedBuffTime;

	bool myIsActivated;
	bool myHaveBeenDeactivated;
};

inline void BlessingTowerComponent::SetGiveManaAmount(ManaPoint aManaAmount)
{
	myGiveManaAmount = aManaAmount;
}

inline void BlessingTowerComponent::SetDamageModifier(float aDamageModifier)
{
	myDamageBonusStats->BonusDamageModifier = aDamageModifier / 100.0f;
}

inline void BlessingTowerComponent::SetDuration(float aDuration)
{
	myDuration = aDuration;
}