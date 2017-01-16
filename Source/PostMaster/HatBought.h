#pragma once
#include "Event.h"
class HatBought : public Event
{
public:
	HatBought(const std::string& aHatName);
	~HatBought();
	
	eMessageReturn DoEvent(CPlayState* aPlayState) const override;
private:
	std::string myHatName;
};