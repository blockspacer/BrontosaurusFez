#pragma once
#include "Event.h"

class CQueryShopItemButtonHovered : public Event
{
public:
	CQueryShopItemButtonHovered(const int aShopItemIndex, std::string* aTooltipTextOut);
	~CQueryShopItemButtonHovered();

	eMessageReturn DoEvent(HatShopState* aHatShopState) const override;

private:
	const int myShopItemIndex;
	std::string* myTooltipTextOut;
};

