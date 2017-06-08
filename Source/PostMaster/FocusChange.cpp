#include "stdafx.h"
#include "FocusChange.h"

#include "BrontosaurusEngine/InputManager.h"
#include "BrontosaurusEngine/DXFramework.h"

FocusChange::FocusChange(const bool aHasFocus)
{
	myHasFocus = aHasFocus;
}

FocusChange::~FocusChange()
{
}

eMessageReturn FocusChange::DoEvent(CInputManager* aInputManager) const
{
	if (aInputManager)
	{
		aInputManager->LockUnlockMouse(myHasFocus);
	}

	return eMessageReturn::eContinue;
}

eMessageReturn FocusChange::DoEvent(CDXFramework* aFramework) const
{
	if (aFramework)
	{
		if (myHasFocus)
		{
			aFramework->SetFullscreen();
		}
		else
		{
			aFramework->SetWindowed();
		}
	}

	return eMessageReturn::eContinue;
}
