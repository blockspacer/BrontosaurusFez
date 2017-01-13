#pragma once
#include "Event.h"
class HatBought : public Event
{
public:
	HatBought();
	~HatBought();
	
	eMessageReturn DoEvent(CPlayState* aPlayState) const override;
private:
};

