#include "stdafx.h"
#include "ShopItemButtonPressed.h"
#include "Game/HatShopState.h"

ShopItemButtonPressed::ShopItemButtonPressed(const char aIndex)
{
	mySelectionIndex = aIndex;
}

ShopItemButtonPressed::~ShopItemButtonPressed()
{
}

eMessageReturn ShopItemButtonPressed::DoEvent(HatShopState * aShopState) const
{
	aShopState->SetSelected(mySelectionIndex);
	return eMessageReturn::eContinue;
}
