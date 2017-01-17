#pragma once
#include "Event.h"

class CPlayerHealthChanged : public Event
{
public:
	CPlayerHealthChanged(const float aNewHealthPercentage);
	~CPlayerHealthChanged();

	eMessageReturn DoEvent(GUI::CHealthWidget* aHealthWidget) const override;

private:
	const float myNewHealthPercentage;
};
