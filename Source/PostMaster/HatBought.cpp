#include "stdafx.h"
#include "HatBought.h"

#include "Game/PlayState.h"

HatBought::HatBought()
{
}


HatBought::~HatBought()
{
}

eMessageReturn HatBought::DoEvent(CPlayState * aPlayState) const
{
	aPlayState->GiveHatToPlayer();
	return eMessageReturn::eContinue;
}
