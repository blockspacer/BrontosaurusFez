#pragma once
#include "Event.h"
class HatBought : public Event
{
public:
	HatBought(const std::string& aHatName);
	~HatBought();
	
	eMessageReturn DoEvent(CHatMaker* aPlayState) const override;
	eMessageReturn DoEvent(CShopStorage* aPlayState) const override;
private:
	std::string myHatName;
};