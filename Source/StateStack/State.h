#pragma once

class StateStack;

namespace CU
{
	class Time;
} 

class State
{
public:
	enum class eStatus : unsigned char
	{
		eKeep,
		ePop
	};

	State(StateStack& aStateStack);

	virtual ~State();

	virtual void Init() = 0;
	virtual eStatus Update(const CU::Time& aDeltaTime) = 0;
	virtual void Render() = 0;

	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;
	virtual bool GetLetThroughRender() const;
	virtual bool GetLetThroughUpdate() const;

	void SetStateStatus(const eStatus aStatus);

protected:
	StateStack& myStateStack;
	eStatus myStatus;
};
