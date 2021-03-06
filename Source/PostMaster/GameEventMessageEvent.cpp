#include "stdafx.h"
#include "GameEventMessageEvent.h"
#include "Game/GameEventMessenger.h"

CGameEventMessageEvent::CGameEventMessageEvent(CU::GrowingArray<CU::DynamicString> aMessage)
{
	myMessage = aMessage;
}

CGameEventMessageEvent::~CGameEventMessageEvent()
{
}

eMessageReturn CGameEventMessageEvent::DoEvent(CGameEventMessenger* aGameEventMessenger) const
{
	aGameEventMessenger->AddMessage(myMessage);
	return eMessageReturn::eContinue;
}
