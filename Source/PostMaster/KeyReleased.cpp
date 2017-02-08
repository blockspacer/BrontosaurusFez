#include "stdafx.h"
#include "KeyReleased.h"

#include "../BrontosaurusEngine/DebugInfoDrawer.h"
#include "../Components/InputController.h"
#include "../Game/PlayState.h"


KeyReleased::KeyReleased(const CU::eKeys& aKey)
	: myKey(aKey)
{
}

KeyReleased::~KeyReleased()
{
}

eMessageReturn KeyReleased::DoEvent(CDebugInfoDrawer* aDebugInfoDrawer) const
{
	if (aDebugInfoDrawer != nullptr)
	{
		aDebugInfoDrawer->ReleasedKey(myKey);
	}

	return eMessageReturn::eContinue;
}

eMessageReturn KeyReleased::DoEvent(InputController * aInputController) const
{
	return aInputController->TakeKeyReleased(myKey);
}


eMessageReturn KeyReleased::DoEvent(CPlayState * aPlayState) const
{
	aPlayState->CameraMovement(myKey, false);
	return eMessageReturn::eContinue;

}