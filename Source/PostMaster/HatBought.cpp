#include "stdafx.h"
#include "HatBought.h"

#include "Game/PlayState.h"

HatBought::HatBought(const std::string& aHatName)
{
	myHatName = aHatName;
}


HatBought::~HatBought()
{
}

eMessageReturn HatBought::DoEvent(CPlayState * aPlayState) const
{
	aPlayState->GiveHatToPlayer();
	return eMessageReturn::eContinue;
}
