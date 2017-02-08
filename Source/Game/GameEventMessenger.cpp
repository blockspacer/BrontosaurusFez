#include "stdafx.h"
#include "GameEventMessenger.h"
#include "PostMaster/Message.h"
#include "PostMaster/Event.h"


CGameEventMessenger::CGameEventMessenger(): myInTweener(nullptr), myOutTweener(nullptr), myCurrentTime(0), myWaitTime(2.f)
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
	CU::Vector4f colour = myText.GetColor();

	if (myInTweener->IsFinished() == false)
	{
		myInTweener->Update((aDeltaTime));
		colour.a = myInTweener->GetValue();
	}
	else if(myCurrentTime < myWaitTime)
	{
		myCurrentTime += aDeltaTime;
	}
	else
	{
		myOutTweener->Update(aDeltaTime);
		colour.a = myOutTweener->GetValue();
	}

	myText.SetColor(colour);
}

void CGameEventMessenger::Render() 
{
	if (myOutTweener->IsFinished() != true)
	{
		myText.Render();
	}
}

void CGameEventMessenger::AddMessage(CU::GrowingArray<CU::DynamicString> someText)
{
	myTextQueue.Push(someText);
}

void CGameEventMessenger::SetMessage(CU::GrowingArray<CU::DynamicString> someStrings)
{
	myInTweener = new CU::Tween(CU::TweenType::Quadratic, CU::TweenMod::EaseInOut, 0.f, 1.f, 2.f);
}
