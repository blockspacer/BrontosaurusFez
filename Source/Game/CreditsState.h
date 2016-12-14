#pragma once
#include "StateStack\State.h"
#include "../PostMaster/Subscriber.h"

class CSpriteInstance;


class CreditsState : public State, public Subscriber
{
public:
	CreditsState(StateStack& aStateStack);
	~CreditsState();

	// Inherited via State
	void Init() override;

	State::eStatus Update(const CU::Time & aDeltaTime) override;
	void Render() override;
	void OnEnter() override;
	void OnExit() override;
	void GoToMainMenu();

	eMessageReturn Recieve(const Message& aMessage) override;
private:
	CSpriteInstance* mysprite;

};

