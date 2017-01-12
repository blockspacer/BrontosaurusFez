#include "stdafx.h"
#include "HatShopState.h"
#include "Game/PollingStation.h"
#include "PostMaster/PostMaster.h"
#include "PostMaster/Message.h"
#include "PostMaster/PopCurrentState.h"
#include "Components/GameObject.h"
#include "Components/GameObjectManager.h"
#include "PostMaster/HatBought.h"
#include "GUI/GUIManager/GUIManager.h"

HatShopState::HatShopState(StateStack & aStateStack) :
	State(aStateStack)
{
	myGUIManager = new GUI::GUIManager();
	myGUIManager->Init("models/gui/shopWindow.fbx");

	GUI::GUIManager tempMan;
	tempMan.Init("models/gui/buyKnapp.fbx");
	myGUIManager->AddWidget("buyknapp", tempMan.RemoveWidget("buyKnapp"));
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
	myGUIManager->Update(aDeltaTime);
	return myStatus;
}

void HatShopState::Render()
{
	myGUIManager->Render();
}

void HatShopState::OnEnter()
{
	PostMaster::GetInstance().Subscribe(this, eMessageType::eKeyPressed);
	myGUIManager->RestartRenderAndUpdate();
}

void HatShopState::OnExit()
{
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eKeyPressed);
	myGUIManager->RestartRenderAndUpdate();
}

void HatShopState::BuyHat()
{
	PostMaster::GetInstance().SendLetter(eMessageType::eHatAdded,HatBought());
}

bool HatShopState::GetLetThroughRender() const
{
	return true;
}

eMessageReturn HatShopState::Recieve(const Message & aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}