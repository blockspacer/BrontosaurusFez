#pragma once
#include "StateStack/State.h"
#include "PostMaster/Subscriber.h"

class HatShopState : public State, public Subscriber
{
public:
	HatShopState(StateStack& aStateStack);
	~HatShopState();

	void CloseShop();

	// Inherited via State
	void Init() override;
	eStatus Update(const CU::Time & aDeltaTime) override;
	void Render() override;
	void OnEnter() override;
	void OnExit() override;
	bool GetLetThroughRender() const override;

	// Inherited via Subscriber
	eMessageReturn Recieve(const Message & aMessage) override;
};