#pragma once
#include "Event.h"

class CPlayerManaChanged : public Event
{
public:
	CPlayerManaChanged(const float aNewManaPercentage);
	~CPlayerManaChanged();

	eMessageReturn DoEvent(GUI::CManaWidget* aManaWidget) const override;

private:
	const float myNewManaPercentage;
};
