#include "stdafx.h"
#include "CreditsState.h"
#include "PostMaster\PostMaster.h"
#include "PostMaster\PopCurrentState.h"
#include "BrontosaurusEngine\ModelInstance.h"
#include "BrontosaurusEngine\SpriteInstance.h"
#include "CommonUtilities\CUTime.h"
#include "BrontosaurusEngine\ParticleEmitterInstance.h"
#include "../PostMaster/Message.h"

CreditsState::CreditsState(StateStack& aStateStack)
	: State(aStateStack)
{
}

CreditsState::~CreditsState()
{
	delete mysprite;
	mysprite = nullptr;
}

void CreditsState::Init()
{
}

eStateStatus CreditsState::Update(const CU::Time & aDeltaTime)
{
	if (mysprite->GetPosition().y > 0)
	{
		mysprite->SetPosition(CU::Vector2f(mysprite->GetPosition().x, mysprite->GetPosition().y + -0.2f * aDeltaTime.GetSeconds()));
	}
	else
	{
		mysprite->SetPosition(CU::Vector2f::Zero);
	}

	return myStatus;
}

void CreditsState::Render()
{
	mysprite->Render();
}

void CreditsState::OnEnter()
{
	//PostMaster::GetInstance().Subscribe(this, eMessageType::eStateMessage);
	PostMaster::GetInstance().Subscribe(this, eMessageType::eKeyboardMessage);
	mysprite = new CSpriteInstance("Models/credits/CreditsScreen.dds",CU::Vector2f(1, 1),CU::Vector2f(0,1));
}

void CreditsState::OnExit()
{
	//PostMaster::GetInstance().UnSubscribe(this, eMessageType::eStateMessage);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eKeyboardMessage);
}

void CreditsState::GoToMainMenu()
{
	PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PopCurrentState()));
}

eMessageReturn CreditsState::Recieve(const Message & aMessage)
{
	//if (aMessage.myMessageType == eMessageType::eStateMessage)
	//{
	//	//TODO::CARL FIX THIST SHIT THING POSTMASTER 2H1t
	//	return aMessage.myEvent.DoEvent(static_cast<State*>(this));
	//}

	return aMessage.myEvent.DoEvent(this);
}
