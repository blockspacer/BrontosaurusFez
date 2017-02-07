#include "stdafx.h"
#include "GoldChanged.h"
#include "Game/PollingStation.h"
#include "Game/PlayState.h"

GoldChanged::GoldChanged(const int aValue, const bool aDecreaseGold)
{
	myValue = aValue;
	myDecreaseGold = aDecreaseGold;
}

GoldChanged::~GoldChanged()
{
}

eMessageReturn GoldChanged::DoEvent(CPlayState* aPlayState) const
{
	aPlayState->ChangeGoldAmount(myValue,myDecreaseGold);
	return eMessageReturn::eStop;
}
