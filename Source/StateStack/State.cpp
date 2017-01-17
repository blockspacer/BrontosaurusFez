#include "stdafx.h"
#include "State.h"

#include "../BrontosaurusEngine/Engine.h"
#include "../BrontosaurusEngine/Renderer.h"


State::State(StateStack & aStateStack)
	: myStateStack(aStateStack)
	, myStatus(eStatus::eKeep)
{
}

State::~State()
{
	RENDERER.ClearRenderQueue();
}

bool State::GetLetThroughRender() const
{
	return false;
}

bool State::GetLetThroughUpdate() const
{
	return false;
}

void State::SetStateStatus(const eStatus aStatus)
{
	myStatus = aStatus;
}
