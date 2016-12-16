#pragma once
#include "Component.h"
#include <GrowingArray.h>

class Hat;

class InventoryComponent : public CComponent
{
public:
	InventoryComponent();
	~InventoryComponent();

	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;
	void AddHat(const Hat& aHat);

private:
	CU::GrowingArray<Hat> myHats;
	unsigned int myGold;
};

