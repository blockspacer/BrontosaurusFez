#pragma once
#include "../PostMaster/Subscriber.h"
#include "../CommonUtilities/stack.h"

#include <functional>

class State;

namespace CU
{
	class Time;
}

const bool STATESTACK_CONTINUE = true;
const bool STATESTACK_QUIT = false;

class StateStack : public Subscriber
{
public:
	StateStack();
	~StateStack();
	void PushState(State *aState);
	State* GetCurrentState();
	bool Update(const CU::Time& aDeltaTime);
	void Render();
	void Pop();
	void Clear();

	void AddSwapStateFunction(const std::function<void(void)>& aSwapStateCunction);
	void SwapState(State* aState);

	eMessageReturn Recieve(const Message& aMessage) override;

private:
	CU::Stack <State*, /*unsigned*/ short> myStates;
	std::function<void(void)> mySwapStateFunction;

	State* myStateToSwapTo;
};
