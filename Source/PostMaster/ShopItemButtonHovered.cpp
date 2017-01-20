#include "stdafx.h"
#include "ShopItemButtonHovered.h"
#include "Game\HatShopState.h"


CQueryShopItemButtonHovered::CQueryShopItemButtonHovered(const int aShopItemIndex, std::string* aTooltipTextOut)
	: myShopItemIndex(aShopItemIndex)
	, myTooltipTextOut(aTooltipTextOut)
{
}

CQueryShopItemButtonHovered::~CQueryShopItemButtonHovered()
{
}

eMessageReturn CQueryShopItemButtonHovered::DoEvent(HatShopState* aHatShopState) const
{
	if (aHatShopState != nullptr)
	{
		if (myTooltipTextOut != nullptr)
		{
			aHatShopState->GetTooltipTextFromShopIndex(myShopItemIndex, *myTooltipTextOut);
		}
	}

	return eMessageReturn::eContinue;
}
