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
#include "../Components/CollisionComponentManager.h"

#include "../Game/QuestManager.h"
#include "../Game/QuestDrawer.h"
#include "..\Game\LevelManager.h"

//temp
#include "Components\HealthComponent.h"
#include "../BrontosaurusEngine/Renderer.h"
#include "Game/DialogState.h"
#include "PostMaster.h"
#include "PushState.h"

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
			//aPlayState->BuyHats();
			return eMessageReturn::eContinue;
			break;
		case CU::eKeys::F7:
			//aPlayState->NextLevel();
			CLevelManager::GetInstance()->GoToLevel(2);
			break;
		case CU::eKeys::N:
			aPlayState->myShuldRenderNavmesh = !aPlayState->myShuldRenderNavmesh;
			break;
		case CU::eKeys::ESCAPE:
			aPlayState->Pause();
			return eMessageReturn::eStop;
			break;
		case CU::eKeys::H:
//			aPlayState->TEMP_ADD_HAT(aPlayState->myPlayerObject);
		default:
			break;
		}
	}

	return eMessageReturn::eContinue;
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

eMessageReturn KeyPressed::DoEvent(CCollisionComponentManager* aCollisionComponentManager) const
{
	if (myKey == CU::eKeys::F3)
	{
		if (aCollisionComponentManager != nullptr)
		{
			aCollisionComponentManager->FlipShouldRender();
		}
	}

	return eMessageReturn::eContinue;
}

eMessageReturn KeyPressed::DoEvent(CDialogState* aDialogState) const
{
	if (myKey != CU::eKeys::ESCAPE)
	{
		aDialogState->Next();
	}
	else
	{
		PostMaster::GetInstance().SendLetter(eMessageType::eStateStackMessage, PushState(PushState::eState::ePauseScreen, 0));
	}
	return eMessageReturn::eStop;
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
//#error "detta är temp-kod, man ska för fan inte kunna stänga av bloom med b in game, mvh carl"
#endif // _RETAIL_BUILD


	switch (myKey)
	{
	case CU::eKeys::B:
		//aRenderer->GetSettings().Bloom = !aRenderer->GetSettings().Bloom;
		break;
	default:
		break;
	}

	return eMessageReturn::eContinue;
}

eMessageReturn KeyPressed::DoEvent(HatShopState * aHatShop) const
{
	switch (myKey)
	{
	case CU::eKeys::ONE:
		aHatShop->SetSelected(0);
		break;
	case CU::eKeys::TWO:
		aHatShop->SetSelected(1);
		break;
	case CU::eKeys::THREE:
		aHatShop->SetSelected(2);
		break;
	case CU::eKeys::FOUR:
		aHatShop->SetSelected(3);
		break;
	case CU::eKeys::FIVE:
		aHatShop->SetSelected(4);
		break;
	case CU::eKeys::SIX:
		aHatShop->SetSelected(5);
		break;
	case CU::eKeys::SEVEN:
		aHatShop->SetSelected(6);
		break;
	case CU::eKeys::EIGHT:
		aHatShop->SetSelected(8);
		break;
	case CU::eKeys::NINE:
		aHatShop->SetSelected(9);
		break;
	case CU::eKeys::ZERO:
		aHatShop->SetSelected(10);
		break;
	case CU::eKeys::ESCAPE:
		aHatShop->CloseShop();
		break;
	}
	return eMessageReturn::eContinue;
}
