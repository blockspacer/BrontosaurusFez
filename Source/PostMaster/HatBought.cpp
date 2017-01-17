#include "stdafx.h"
#include "HatBought.h"
#include "Game/HatMaker.h"
#include "Game/ShopStorage.h"

HatBought::HatBought(const std::string& aHatName)
{
	myHatName = aHatName;
}


HatBought::~HatBought()
{
}

eMessageReturn HatBought::DoEvent(CHatMaker * aHatMaker) const
{
	aHatMaker->MakeHatFromBluePrint(myHatName);
	return eMessageReturn::eContinue;
}

eMessageReturn HatBought::DoEvent(CShopStorage * aPlayState) const
{
	aPlayState->RemoveHat(myHatName);
	return eMessageReturn::eContinue;
}
