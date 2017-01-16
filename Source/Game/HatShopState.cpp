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
#include "Components/PlayerData.h"

HatShopState::HatShopState(StateStack & aStateStack) :
	State(aStateStack)
{
	myCurrentlySelected = nullptr;
	myGUIManager = new GUI::GUIManager();
	myGUIManager->Init("models/gui/shopWindow.fbx");

	GUI::GUIManager tempMan;
	tempMan.Init("models/gui/buyKnapp.fbx");
	myGUIManager->AddWidget("buyknapp", tempMan.RemoveWidget("buyKnapp"));
	mySelections.Init(3);

	SShopSelection* selection1 = new SShopSelection();
	selection1->HatName = "firsthat";
	selection1->myCost = 50;
	mySelections.Add(*selection1);

	SShopSelection* selection2 = new SShopSelection();
	selection2->HatName = "secondhat";
	selection2->myCost = 100;
	mySelections.Add(*selection2);

	SShopSelection* selection3 = new SShopSelection();
	selection3->HatName = "thirdhat";
	selection3->myCost = 150;
	mySelections.Add(*selection3);
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
	myCurrentlySelected = &mySelections[0];
	return myStatus;
}

void HatShopState::Render()
{
	myGUIManager->Render();
}

void HatShopState::OnEnter()
{
	PostMaster::GetInstance().Subscribe(this, eMessageType::eKeyPressed);
	PostMaster::GetInstance().Subscribe(this, eMessageType::eBuyButtonPressed);
	myGUIManager->RestartRenderAndUpdate();
	myCurrentlySelected = nullptr;
}

void HatShopState::OnExit()
{
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eKeyPressed);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eBuyButtonPressed);
	myGUIManager->PauseRenderAndUpdate();
}

void HatShopState::ValidatePurchase()
{
	short playerWallet = PollingStation::playerData->myGold;
	if (myCurrentlySelected != nullptr)
	{
		if (playerWallet >= myCurrentlySelected->myCost)
		{
			PostMaster::GetInstance().SendLetter(eMessageType::eHatAdded,HatBought(myCurrentlySelected->HatName));
			PollingStation::playerData->myGold -= myCurrentlySelected->myCost;
			mySelections.Remove(*myCurrentlySelected);
			myCurrentlySelected = nullptr;
		}
	}
}

bool HatShopState::GetLetThroughRender() const
{
	return true;
}

eMessageReturn HatShopState::Recieve(const Message & aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}