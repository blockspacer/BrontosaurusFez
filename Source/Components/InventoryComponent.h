#pragma once
#include "Component.h"
#include <GrowingArray.h>

class CHat;

class CInventoryComponent : public CComponent
{
public:
	CInventoryComponent();
	~CInventoryComponent();

	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;
	void AddHat(CHat& aHat);

private:
	CU::GrowingArray<CHat> myHats;
	unsigned int myGold;
};

