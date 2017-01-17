#include "stdafx.h"
#include "HatShopState.h"
#include "Game/PollingStation.h"
#include "PostMaster/PostMaster.h"
#include "PostMaster/Message.h"
#include "PostMaster/PopCurrentState.h"
#include "Components/GameObject.h"
#include "Components/GameObjectManager.h"
#include "Components/PlayerData.h"
#include "PostMaster/HatBought.h"
#include "KevinLoader/KevinLoader.h"
#include "GUI/Widget/Widget.h"
#include "GUI/GUIManager/GUIManager.h"

HatShopState::HatShopState(StateStack & aStateStack) :
	State(aStateStack)
{
	myCurrentlySelected = nullptr;
	myGUIManager = new GUI::GUIManager();
	myGUIManager->Init("models/gui/shopWindow.fbx");
	GUI::GUIManager tempMan;
	tempMan.Init("models/gui/buyKnapp.fbx");
	myGUIManager->AddWidget("buyknapp", tempMan.RemoveWidget("buyKnapp") );
	mySelections.Init(3);
	CU::CJsonValue value;
	const std::string& errorString = value.Parse("Json/Hats/HatBluePrints.json");

	CU::CJsonValue hatsArray = value.at("Hats");

	for (unsigned int i = 0; i < hatsArray.Size(); ++i)
	{
		if (hatsArray[i].at("AvailableInShop").GetBool() == true)
		{
			SShopSelection* shopSelection = new SShopSelection();
			shopSelection->HatName = hatsArray[i].at("HatName").GetString();
			shopSelection->myCost = hatsArray[i].at("ShopCost").GetInt();
			mySelections.Add(shopSelection);
		}
	}
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
		if (playerWallet <= myCurrentlySelected->myCost)
		{
			PostMaster::GetInstance().SendLetter(eMessageType::eHatAdded,HatBought(myCurrentlySelected->HatName));
			PollingStation::playerData->myGold -= myCurrentlySelected->myCost;
			mySelections.DeleteCyclic(myCurrentlySelected);
			myCurrentlySelected = nullptr;
		}
	}
}

void HatShopState::SetSelected(unsigned int aIndex)
{
	if (aIndex >= mySelections.Size())
	{
		aIndex = mySelections.Size() - 1;
	}
	myCurrentlySelected = mySelections[aIndex];
}

bool HatShopState::GetLetThroughRender() const
{
	return true;
}

bool HatShopState::GetLetThroughUpdate() const
{
	return true;
}

eMessageReturn HatShopState::Recieve(const Message & aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}