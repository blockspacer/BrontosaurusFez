#include "stdafx.h"
#include "HatShopState.h"
#include "PostMaster/PostMaster.h"
#include "PostMaster/Message.h"
#include "PostMaster/PopCurrentState.h"

HatShopState::HatShopState(StateStack & aStateStack) :
	State(aStateStack)
{
}

HatShopState::~HatShopState()
{
}

void HatShopState::CloseShop()
{
	SetStateStatus(eStatus::ePop);
}

void HatShopState::Init()
{
}

State::eStatus HatShopState::Update(const CU::Time & aDeltaTime)
{
	int br = 0;
	br++;


	return myStatus;
}

void HatShopState::Render()
{
}

void HatShopState::OnEnter()
{
	PostMaster::GetInstance().Subscribe(this, eMessageType::eKeyPressed);
}

void HatShopState::OnExit()
{
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eKeyPressed);
}

bool HatShopState::GetLetThroughRender() const
{
	return true;
}

eMessageReturn HatShopState::Recieve(const Message & aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}