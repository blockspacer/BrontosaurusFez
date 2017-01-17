#include "stdafx.h"
#include "BuyButtonPressed.h"
#include "Game/HatShopState.h"

BuyButtonPressed::BuyButtonPressed()
{
}


BuyButtonPressed::~BuyButtonPressed()
{
}

eMessageReturn BuyButtonPressed::DoEvent(HatShopState *aHatState) const
{
	aHatState->ValidatePurchase();
	return eMessageReturn::eContinue;
}
