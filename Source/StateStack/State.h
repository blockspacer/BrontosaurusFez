#pragma once

#include "../PostMaster/Subscriber.h"
#include "../BrontosaurusEngine/Engine.h"
#include "../BrontosaurusEngine/Renderer.h"

class StateStack;

namespace CU
{
	class Time;
} 

class State// : public Subscriber
{
public:
	enum class eStatus : unsigned char
	{
		eKeep,
		ePop
	};

	State(StateStack& aStateStack) : myStateStack(aStateStack), myStatus(eStatus::eKeep)
	{
	}

	virtual ~State()
	{
		RENDERER.ClearRenderQueue();
	}

	virtual void Init() = 0;
	virtual eStatus Update(const CU::Time& aDeltaTime) = 0;
	virtual void Render() = 0; //Render används i StateStack la till denna. Alex 16-10-14

	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;
	virtual bool GetLetThroughRender() const;

	inline void SetStateStatus(const eStatus aStatus);

	//inline eMessageReturn Recieve(const Message& aMessage) override;

protected:
	StateStack& myStateStack;
	eStatus myStatus;
};

inline bool State::GetLetThroughRender() const
{
	return false;
}

inline void State::SetStateStatus(const eStatus aStatus)
{
	myStatus = aStatus;
}

//#include "../PostMaster/Message.h" //TODO: we should make a cpp file if this is to be kept
//#include "../PostMaster/Event.h"
//inline eMessageReturn State::Recieve(const Message& aMessage)
//{
//	return aMessage.myEvent.DoEvent(this);
//}
