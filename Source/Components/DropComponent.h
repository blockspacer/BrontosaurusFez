#pragma once
#include "Component.h"


class DropComponent : public CComponent
{
public:
	DropComponent(const int aAmountOfGoldToDrop, const int aHealthDropPercentChance, const int aManaDropPercentChance, const char* aHatToDrop);
	~DropComponent();

	void Update(float aDeltaTime);
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;


private:
	bool CalculateDropHealthGlobe();
	bool CalculateDropManaGlobe();

private:
	int myGoldToDrop;
	float myHealthGlobeDropRate;
	float myManaGlobeDropRate;
	std::string myHatToDrop;
};

