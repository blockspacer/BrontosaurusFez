#include "stdafx.h"
#include "PushState.h"

#include "StateStack/StateStack.h"
#include "Game/PlayState.h"
#include "Game/CreditsState.h"
#include "Game/PauseMenu.h"
#include "Game/HatShopState.h"
#include "Game/LevelSelectState.h"
#include "Game/MainMenuState.h"
#include "Game/LoadState.h"
#include "Game/DialogState.h"

PushState::PushState(eState aState, const int aLevelIndex)
	: myState(aState)
	, myLevelIndex(aLevelIndex)
{
}

PushState::~PushState()
{
}

eMessageReturn PushState::DoEvent(StateStack* aStateStack) const
{
	if (aStateStack != nullptr)
	{
		switch (myState)
		{
		case eState::ePlayState:
			if (myLevelIndex >= 10)
			{
				aStateStack->PushState(new CLoadState(*aStateStack, myLevelIndex % 10, true));
				break;
			}

			aStateStack->PushState(new CLoadState(*aStateStack, myLevelIndex));
			break;
		case eState::eDeathScreen:
			//aStateStack->PushState(new CDeathScreenState(*aStateStack));
			break;
		case eState::eCreditScreen:
			aStateStack->PushState(new CreditsState(*aStateStack, static_cast<bool>(myLevelIndex)));
			break;
		case eState::ePauseScreen:
			aStateStack->PushState(new PauseMenu(*aStateStack));
			break;
		case eState::eHatShop:
			aStateStack->PushState(new HatShopState(*aStateStack));
			break;
		case eState::eLevelSelect:
			static_cast<MainMenuState*>(aStateStack->GetCurrentState())->SetIsGoingToLevelSelect(true);
			aStateStack->PushState(new LevelSelectState(*aStateStack));
			break;
		case eState::eDialog:
       			aStateStack->PushState(new CDialogState(*aStateStack));
			break;
		default: break;
		}
	}

	return eMessageReturn::eStop;
}
