#pragma once
#include "..\StateStack\State.h"
#include "PostMaster\Subscriber.h"

namespace GUI
{
	class GUIManager;
}

class LevelSelectState : public State, public Subscriber
{
public:
	LevelSelectState(StateStack& aStateStack);
	~LevelSelectState();

	void Init() override;
	eStateStatus Update(const CU::Time& aDeltaTime) override;
	void Render() override;
	void OnEnter() override;
	void OnExit() override;

	bool GetLetThroughRender() const override;
	eMessageReturn Recieve(const Message & aMessage) override;

private:
	GUI::GUIManager* myGUIManager;
};
