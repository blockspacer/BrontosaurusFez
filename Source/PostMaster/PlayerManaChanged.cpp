#include "stdafx.h"
#include "PlayerManaChanged.h"
#include "..\GUI\ManaWidget.h"

CPlayerManaChanged::CPlayerManaChanged(const float aNewManaPercentage)
	: myNewManaPercentage(aNewManaPercentage)
{
}

CPlayerManaChanged::~CPlayerManaChanged()
{
}

eMessageReturn CPlayerManaChanged::DoEvent(GUI::CManaWidget* aManaWidget) const
{
	if (aManaWidget != nullptr)
	{
		aManaWidget->SetManaPercent(myNewManaPercentage);
	}

	return eMessageReturn::eContinue;
}
