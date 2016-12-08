#include "stdafx.h"
#include "ChangeLevel.h"
#include "Game\LoadState.h"
#include "..\StateStack\StateStack.h"
#include "..\Game\PlayState.h"

ChangeLevel::ChangeLevel(const int aNewLevelIndex)
	: myNewLevelIndex(aNewLevelIndex)
{
}

ChangeLevel::~ChangeLevel()
{
}

eMessageReturn ChangeLevel::DoEvent(StateStack* aStateStack) const
{
	if (aStateStack != nullptr)
	{
		//int levelIndex = myNewLevelIndex;
		//auto changeLevel = [aStateStack, levelIndex]
		//{
		//	//aStateStack->Pop(); //pop victory screen
		//	//aStateStack->Pop(); //pop play state
		aStateStack->SwapState(new CLoadState(*aStateStack, myNewLevelIndex));
		//	//aStateStack->PushState(new CPlayState(*aStateStack, levelIndex));
		//};
	   //
		//aStateStack->AddSwapStateFunction(changeLevel);
	}

	return eMessageReturn::eStop;
}
