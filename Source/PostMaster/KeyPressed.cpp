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
#include "../Components/InputController.h"
#include "Game/HatShopState.h"

//temp
#include "Components\HealthComponent.h"
#include "../BrontosaurusEngine/Renderer.h"

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
		case CU::eKeys::S:
			aPlayState->BuyHats();
			break;
		case CU::eKeys::F7:
			aPlayState->NextLevel();
			break;
		case CU::eKeys::P:
		case CU::eKeys::ESCAPE:
			aPlayState->Pause();
			break;
		case CU::eKeys::H:
			aPlayState->TEMP_ADD_HAT(aPlayState->myPlayerObject);
		case CU::eKeys::F11:
			{
				QM::CQuestManager& questManager = QM::CQuestManager::GetInstance();
				questManager.UpdateObjective(aPlayState->fristObjective);
				questManager.UpdateObjective(aPlayState->secondObjective);
				questManager.UpdateObjective(aPlayState->thridObjective);
			}
			break;
		case CU::eKeys::RETURN:
		{
			QM::CQuestManager& questManager = QM::CQuestManager::GetInstance();
			questManager.CompleteEvent();
		}
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

eMessageReturn KeyPressed::DoEvent(CConsole* /*aConsole*/) const
{
	//aConsole->TakeKeyBoardInputPressedChar(myKey);
	return eMessageReturn::eContinue;
}

eMessageReturn KeyPressed::DoEvent(InputController * aInputController) const
{
	return aInputController->TakeKeyPressed(myKey);
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

eMessageReturn KeyPressed::DoEvent(CRenderer* aRenderer) const
{
	if (aRenderer == nullptr)
	{
		return eMessageReturn::eContinue;
	}

#ifdef _RETAIL_BUILD
#error "detta är temp-kod, man ska för fan inte kunna stänga av bloom med b in game, mvh carl"
#endif // _RETAIL_BUILD


	switch (myKey)
	{
	case CU::eKeys::B:
		aRenderer->GetSettings().Bloom = !aRenderer->GetSettings().Bloom;
		break;
	default:
		break;
	}

	return eMessageReturn::eContinue;
}

eMessageReturn KeyPressed::DoEvent(HatShopState * aHatShop) const
{
	int br = 0;
	br++;
	switch (myKey)
	{
	case CU::eKeys::ESCAPE:
		aHatShop->CloseShop();
		break;
	}
	return eMessageReturn::eContinue;
}
