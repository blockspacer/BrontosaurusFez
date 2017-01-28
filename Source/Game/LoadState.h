#pragma once
#include "StateStack/State.h"
#include "LoadingAnimation.h"

class CPlayState;

class CLoadState :public State
{
public:
	CLoadState(StateStack& aStateStack, const int aLevelIndex, const bool aShouldReturnToLevelSelect = false);
	~CLoadState();

	void Init() override;
	eStateStatus Update(const CU::Time& aDeltaTime) override;
	void Render() override;

	void OnEnter() override;
	void OnExit() override;
private:

	CPlayState* myPlayState;
	CLoadingAnimation myLoadingAnimation;

	const int myLevelIndex;
	const bool myShouldReturnToLevelSelect;
};

