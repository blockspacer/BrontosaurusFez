#include "stdafx.h"
#include "PlayerGotHat.h"
#include "../GUI/HatContainer.h"
#include "GUI/GUIManager.h"

CPlayerGotHat::CPlayerGotHat(SHatBluePrint* aHatBluePrint)
	: myHatBluePrint(aHatBluePrint)
{
}

CPlayerGotHat::~CPlayerGotHat()
{
}

eMessageReturn CPlayerGotHat::DoEvent(GUI::GUIManager* aGUIManager) const
{
	if (aGUIManager)
	{
		aGUIManager->UpdateHatIcons();
	}

	return eMessageReturn::eContinue;
}

eMessageReturn CPlayerGotHat::DoEvent(GUI::HatContainer* aGUIHatContainer) const
{
	if (aGUIHatContainer)
	{
		aGUIHatContainer->TryAddHat(myHatBluePrint);
	}

	return eMessageReturn::eContinue;
}
