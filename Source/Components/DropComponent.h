#pragma once
#include "Component.h"
class DropComponent : public CComponent
{
public:
	DropComponent(const int aAmountOfGoldToDrop, const int aDropPercentChance);
	~DropComponent();

	void Update(float aDeltaTime);
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;


private:
	bool CalculateDropHealthGlobe();

private:
	int myGoldToDrop;
	float myHealthGlobeDropRate;
};

