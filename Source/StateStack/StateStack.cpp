#include "stdafx.h"
#include "StateStack.h"


#include "../CommonUtilities/CUTime.h"
#include "../PostMaster/PostMaster.h"
#include "../PostMaster/Event.h"
#include "../PostMaster/Message.h"

StateStack::StateStack()
	: mySwapStateFunction(nullptr), myStateToSwapTo(nullptr)
{
	myShouldUpdate = true;
	PostMaster::GetInstance().Subscribe(this, eMessageType::eStateStackMessage);
	PostMaster::GetInstance().Subscribe(this, eMessageType::eConsoleCalledUpon);
}

StateStack::~StateStack()
{
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eStateStackMessage);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eConsoleCalledUpon);
	Clear();
}

void StateStack::PushState(State *aState)
{
	if(myShouldUpdate == true)
	{
		if (myStates.Size() > 0)
		{
			myStates.Top()->OnExit();
		}
		myStates.Push(aState);
		aState->Init();
		aState->OnEnter();
	
	}
}

State* StateStack::GetCurrentState()
{
	return myStates.Top();
}

const State::eStatus StateStack::UpdateState(const CU::Time & aDeltaTime)
{
	if (myStates.Top()->GetLetThroughUpdate() == true)
	{
		myStates.At(-2)->Update(aDeltaTime);
	}
	return myStates.Top()->Update(aDeltaTime);
}



bool StateStack::Update(const CU::Time& aDeltaTime)
{
	if (myStates.Size() > 0)
	{
		if (myShouldUpdate == true)
		{
			if (UpdateState(aDeltaTime) == State::eStatus::ePop)
			{
				Pop();
			}

			if (mySwapStateFunction != nullptr)
			{
				mySwapStateFunction();
				mySwapStateFunction = nullptr;
			}

			if (myStateToSwapTo != nullptr)
			{
				Pop();
				PushState(myStateToSwapTo);
				myStateToSwapTo = nullptr;
			}
		
		}

		return STATESTACK_CONTINUE;
	}

	return STATESTACK_QUIT;
}

void StateStack::Render()
{
	if (myStates.Size() > 0)
	{
		if (myStates.Top()->GetLetThroughRender() == true)
		{
			myStates.At(-2)->Render();
		}

		myStates.Top()->Render();
	}
}

void StateStack::Pop()
{
	myStates.Top()->OnExit();

	delete myStates.Pop();

	if (myStates.Size() > 0)
	{
		myStates.Top()->OnEnter();
	}
}

void StateStack::Clear()
{
	while (myStates.Size() > 0)
	{
		myStates.Top()->OnExit();
		delete myStates.Pop();
	}
}

void StateStack::AddSwapStateFunction(const std::function<void(void)>& aSwapStateCunction)
{
	mySwapStateFunction = aSwapStateCunction;
}

void StateStack::SwapState(State* aState)
{
	myStateToSwapTo = aState;
}

eMessageReturn StateStack::Recieve(const Message& aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}

