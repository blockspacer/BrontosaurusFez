#include "stdafx.h"
#include "PopCurrentState.h"

#include "../StateStack/State.h"
#include "../StateStack/StateStack.h"

PopCurrentState::PopCurrentState()
{
}

PopCurrentState::~PopCurrentState()
{
}

//eMessageReturn PopCurrentState::DoEvent(State* aCurrentState) const
//{
//	if (aCurrentState != nullptr)
//	{
//		aCurrentState->SetStateStatus(State::eStatus::ePop);
//	}
//
//	return eMessageReturn::eStop;
//}

eMessageReturn PopCurrentState::DoEvent(StateStack* aStateStack) const
{
	if (aStateStack != nullptr)
	{
		aStateStack->GetCurrentState()->SetStateStatus(State::eStatus::ePop);
	}

	return eMessageReturn::eStop;
}
