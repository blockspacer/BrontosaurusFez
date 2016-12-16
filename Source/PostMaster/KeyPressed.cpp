#include "stdafx.h"
#include "KeyPressed.h"

#include "../CommonUtilities/EKeyboardKeys.h"
#include "../BrontosaurusEngine/DebugInfoDrawer.h"
#include "../Components/ModelComponent.h"
#include "Game\CreditsState.h"
#include "../Game/PlayState.h"
#include"Game\PauseMenu.h"
#include "..\BrontosaurusEngine\ModelInstance.h"
#include "../BrontosaurusEngine/Console.h"
#include "Components\HealthComponent.h"

KeyPressed::KeyPressed(const CU::eKeys& aKey)
	: myKey(aKey)
{
}

KeyPressed::~KeyPressed()
{
}

eMessageReturn KeyPressed::DoEvent(CDebugInfoDrawer* aDebugInfoDrawer) const
{
	if (aDebugInfoDrawer != nullptr)
	{
		aDebugInfoDrawer->PressedKey(myKey);
	}

	return eMessageReturn::eContinue;
}

eMessageReturn KeyPressed::DoEvent(CreditsState *aCreditState) const
{
	if (aCreditState != nullptr)
	{
		switch (myKey)
		{
		case CU::eKeys::RETURN:
		case CU::eKeys::ESCAPE:
			aCreditState->GoToMainMenu();
			break;
		default:
			break;
		}
	}

	return eMessageReturn::eContinue;
}

eMessageReturn KeyPressed::DoEvent(CPlayState* aPlayState) const
{
	if (aPlayState != nullptr)
	{
		switch (myKey)
		{
		case CU::eKeys::F7:
			aPlayState->NextLevel();
			break;
		case CU::eKeys::P:
		case CU::eKeys::ESCAPE:
			aPlayState->Pause();
			break;
		default:
			break;
		}
	}

	return eMessageReturn::eStop;
}

eMessageReturn KeyPressed::DoEvent(State* aState) const
{
	CPlayState* playState = dynamic_cast<CPlayState*>(aState);
	if (playState != nullptr)
	{
		switch (myKey)
		{
		case CU::eKeys::F7:
			playState->NextLevel();
			break;
		default:
			break;
		}
	}

	return eMessageReturn::eStop;
}

eMessageReturn KeyPressed::DoEvent(PauseMenu *aPauseMenu) const
{
	switch (myKey)
	{
	case CU::eKeys::P:
	case CU::eKeys::ESCAPE:
		aPauseMenu->Resume();
		break;
	default:
		break;
	}
	return eMessageReturn::eContinue;
}

eMessageReturn KeyPressed::DoEvent(CConsole *aConsole) const
{
	//aConsole->TakeKeyBoardInputPressedChar(myKey);
	return eMessageReturn::eContinue;
}

eMessageReturn KeyPressed::DoEvent(CHealthComponent * aHealthComponent) const
{
	switch (myKey)
	{
	case CU::eKeys::K:
		aHealthComponent->SetHealth(0);
		break;
	default:
		break;
	}
	return eMessageReturn::eContinue;
}
