#include "stdafx.h"
#include "GameEventMessenger.h"
#include "PostMaster/Message.h"
#include "PostMaster/Event.h"


CGameEventMessenger::CGameEventMessenger()
{
}


CGameEventMessenger::~CGameEventMessenger()
{
}

eMessageReturn CGameEventMessenger::Recieve(const Message& aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}

void CGameEventMessenger::Init(const CU::Vector2f& aPosition)
{
	myText.Init();
	myText.SetPosition(aPosition);
	myText.SetTextLines({ "Test !!!!", "Will show useful stuffs as like i don't know when you get one of those hat thignies" });
	myText.SetAlignment(eAlignment::CENTER);
}

void CGameEventMessenger::Update(const float aDeltaTime)
{
	//doesNothing at los momentos
}

void CGameEventMessenger::Render() const
{
	myText.Render();
}

