#include "stdafx.h"
#include "PlayerHealthChanged.h"
#include "..\GUI\HealthWidget.h"

CPlayerHealthChanged::CPlayerHealthChanged(const float aNewHealthPercentage)
	: myNewHealthPercentage(aNewHealthPercentage)
{
}

CPlayerHealthChanged::~CPlayerHealthChanged()
{
}

eMessageReturn CPlayerHealthChanged::DoEvent(GUI::CHealthWidget* aHealthWidget) const
{
	if (aHealthWidget != nullptr)
	{
		aHealthWidget->SetHealthPercent(myNewHealthPercentage);
	}

	return eMessageReturn::eContinue;
}
