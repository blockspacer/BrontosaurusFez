#pragma once
#include "Event.h"
class GoldChanged : public Event
{
public:
	GoldChanged(const int aValue, const bool aDecreaseGold = false);
	~GoldChanged();

	eMessageReturn DoEvent(CPlayState*) const override;
private:
	int myValue;
	bool myDecreaseGold;
};

