#pragma once
#include "Event.h"
class ShopItemButtonPressed : public Event
{
public:
	ShopItemButtonPressed(const char aIndex);
	~ShopItemButtonPressed();


	eMessageReturn DoEvent(HatShopState*) const override;
private:
	char mySelectionIndex;
};

