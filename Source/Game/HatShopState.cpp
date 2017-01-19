#include "stdafx.h"
#include <string>
#include "HatShopState.h"

#include "Game/ShopStorage.h"
#include "Game/PollingStation.h"

#include "PostMaster/Message.h"
#include "PostMaster/PostMaster.h"
#include "PostMaster/PopCurrentState.h"

#include "Components/GameObject.h"
#include "Components/GameObjectManager.h"
#include "Components/PlayerData.h"

#include "PostMaster/HatBought.h"

#include "KevinLoader/KevinLoader.h"

#include "GUI/Widget/Widget.h"
#include "GUI/GUIManager/GUIManager.h"
#include "BrontosaurusEngine/TextInstance.h"

HatShopState::HatShopState(StateStack & aStateStack) :
	State(aStateStack)
{
	myCurrentlySelected = nullptr;
	myGUIManager = new GUI::GUIManager();
	myGUIManager->Init("models/gui/shopWindow.fbx");
	CU::CJsonValue value;
	const std::string& errorString = value.Parse("Json/Hats/HatBluePrints.json");

	CU::CJsonValue hatsArray = value.at("Hats");
	if (CShopStorage::GetInstance().myStorage.HatStorage.Size() != 0)
	{
		myOptionsText.Init(CShopStorage::GetInstance().myStorage.HatStorage.Size());
		myCostText.Init(CShopStorage::GetInstance().myStorage.HatStorage.Size());
		mySelections.Init(CShopStorage::GetInstance().myStorage.HatStorage.Size());
	}
	else
	{
		myOptionsText.Init(1);
		myCostText.Init(1);
		mySelections.Init(1);
	}
	for (unsigned int i = 0; i < CShopStorage::GetInstance().myStorage.HatStorage.Size(); ++i)
	{
		//SShopSelection* shopSelection = new SShopSelection();
		//shopSelection->HatName = CShopStorage::GetInstance().myStorage.HatStorage[i].HatName;
		//shopSelection->myCost = CShopStorage::GetInstance().myStorage.HatStorage[i].myCost;
		//
		//mySelections.Add(shopSelection);
		//myOptionsText.Add(new CTextInstance());
		//myCostText.Add(new CTextInstance());
		//
		//std::string temp;
		//
		//temp += std::to_string(i + 1);
		//temp += ". ";
		//temp += shopSelection->HatName.c_str();
		//myOptionsText.GetLast()->Init();
		//myOptionsText.GetLast()->SetText(temp.c_str());
		//myOptionsText.GetLast()->SetPosition(CU::Vector2f(0.02f, 0.2f + 0.1f * i));
		//
		//temp = "Cost";
		//temp += ": ";
		//temp += std::to_string(shopSelection->myCost);
		//myCostText.GetLast()->Init();
		//myCostText.GetLast()->SetText(temp.c_str());
		//myCostText.GetLast()->SetPosition(CU::Vector2f(0.02f, 0.235f + 0.1f * i));
	}
	for (unsigned int i = 0; i < CShopStorage::GetInstance().myStorage.StorageWithBuyOrder.Size(); ++i)
	{
		if (CShopStorage::GetInstance().myStorage.StorageWithBuyOrder[i].Size() != 0)
		{
			SShopSelection* shopSelection = new SShopSelection();
			shopSelection->HatName = CShopStorage::GetInstance().myStorage.StorageWithBuyOrder[i][0].HatName;
			shopSelection->myCost = CShopStorage::GetInstance().myStorage.StorageWithBuyOrder[i][0].myCost;

			mySelections.Add(shopSelection);
			myOptionsText.Add(new CTextInstance());
			myCostText.Add(new CTextInstance());

			std::string temp;

			temp += std::to_string(i + 1);
			temp += ". ";
			temp += shopSelection->HatName.c_str();
			myOptionsText.GetLast()->Init();
			myOptionsText.GetLast()->SetText(temp.c_str());
			myOptionsText.GetLast()->SetPosition(CU::Vector2f(0.02f, 0.2f + 0.1f * i));

			temp = "Cost";
			temp += ": ";
			temp += std::to_string(shopSelection->myCost);
			myCostText.GetLast()->Init();
			myCostText.GetLast()->SetText(temp.c_str());
			myCostText.GetLast()->SetPosition(CU::Vector2f(0.02f, 0.235f + 0.1f * i));
		}
	}
	AdjustText();
}

HatShopState::~HatShopState()
{
	mySelections.DeleteAll();
	myOptionsText.DeleteAll();
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
	for (unsigned int i = 0; i < myOptionsText.Size(); ++i)
	{
		myOptionsText[i]->Render();
	}
	for (unsigned int i = 0; i < myCostText.Size(); ++i)
	{
		myCostText[i]->Render();
	}
}

void HatShopState::OnEnter()
{
	PostMaster::GetInstance().Subscribe(this, eMessageType::eKeyPressed);
	PostMaster::GetInstance().Subscribe(this, eMessageType::eBuyButtonPressed);
	PostMaster::GetInstance().Subscribe(this, eMessageType::eShopItemSelected);
	myGUIManager->RestartRenderAndUpdate();
	myCurrentlySelected = nullptr;
}

void HatShopState::OnExit()
{
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eKeyPressed);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eBuyButtonPressed);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eShopItemSelected);
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
			mySelections.Delete(myCurrentlySelected);
			myCurrentlySelected = nullptr;
			AdjustText();
		}
	}
}

void HatShopState::SetSelected(const char aIndex)
{
	myCurrentlySelected = nullptr;
	if (mySelections.Size() != 0)
	{
		myCurrentlySelected = mySelections[aIndex];
	}

	//Recolor the texts so you know which one is currently selected
	if (myCurrentlySelected != nullptr)
	{
		for (unsigned int i = 0; i < mySelections.Size(); ++i)
		{
			if (mySelections[i] == myCurrentlySelected)
			{
				for (unsigned int j = 0; j < myOptionsText.Size(); ++j)
				{
					myOptionsText[j]->SetColor(CTextInstance::White);
				}
				myOptionsText[i]->SetColor(CTextInstance::Red);
			}
		}
	}
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

void HatShopState::AdjustText()
{
	myOptionsText.DeleteAll();
	myCostText.DeleteAll();
	for (unsigned int i = 0; i < mySelections.Size(); i++)
	{
		std::string temp;
		temp += std::to_string(i + 1);
		temp += ". ";
		temp += mySelections[i]->HatName.c_str();
		myOptionsText.Add(new CTextInstance());
		myOptionsText.GetLast()->Init();
		myOptionsText.GetLast()->SetText(temp.c_str());
		myOptionsText.GetLast()->SetPosition(CU::Vector2f(0.02f, 0.2f + 0.1f * i));

		temp = "Cost";
		temp += ": ";
		temp += std::to_string(mySelections[i]->myCost);
		myCostText.Add(new CTextInstance());
		myCostText.GetLast()->Init();
		myCostText.GetLast()->SetText(temp.c_str());
		myCostText.GetLast()->SetPosition(CU::Vector2f(0.02f, 0.235f + 0.1f * i));
	}
}
