#pragma once
#include "PostMaster/Event.h"
class BuyButtonPressed : public Event
{
public:
	BuyButtonPressed();
	~BuyButtonPressed();
	eMessageReturn DoEvent(HatShopState*) const override;
private:
};

