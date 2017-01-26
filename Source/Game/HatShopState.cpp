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
#include "PostMaster/ShopClosed.h"

#include "KevinLoader/KevinLoader.h"

#include "GUI/Widget.h"
#include "GUI/GUIManager.h"
#include "BrontosaurusEngine/TextInstance.h"

HatShopState::HatShopState(StateStack & aStateStack) :
	State(aStateStack)
{
	myCurrentlySelected = nullptr;
	myGUIManager = new GUI::GUIManager();
	myGUIManager->Init("models/gui/shopWindow.fbx");
	CU::CJsonValue value;
	std::string errorString = value.Parse("Json/Hats/HatBluePrints.json");

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
	myCostText.DeleteAll();
	delete myGUIManager;
	myGUIManager = nullptr;
	myCurrentlySelected = nullptr;
}

void HatShopState::CloseShop()
{
	SetStateStatus(eStatus::ePop);
	PostMaster::GetInstance().SendLetter(Message(eMessageType::eShopClosed,ShopClosed()));
}

void HatShopState::Init()
{
}

eStateStatus HatShopState::Update(const CU::Time & aDeltaTime)
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
		if (playerWallet >= myCurrentlySelected->myCost)
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
	if (mySelections.Size() != 0 && aIndex < mySelections.Size())
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

void HatShopState::GetTooltipTextFromShopIndex(const int aHatShopIndex, std::string& aTooltipTextOut) const
{
	if (mySelections.Size() > 0 && aHatShopIndex < mySelections.Size())
	{
		const std::string& hoveredHatName = mySelections[aHatShopIndex]->HatName;
		static CU::CJsonValue hatBluePrints("Json/Hats/HatBluePrints.json");

		CU::CJsonValue hatArray = hatBluePrints["Hats"];
		for (int i = 0; i < hatArray.Size(); ++i)
		{
			const std::string& hatName = hatArray[i]["HatName"].GetString();
			if (hoveredHatName == hatName)
			{
				aTooltipTextOut = hatArray[i]["ShopTooltip"].GetString();
				break;
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
	char row = 1;
	for (unsigned int i = 0; i < mySelections.Size(); i++)
	{

		if (i % 2 != 0)
		{
			std::string temp;
			temp += std::to_string(i + 1);
			temp += ". ";
			temp += mySelections[i]->HatName.c_str();
			myOptionsText.Add(new CTextInstance());
			myOptionsText.GetLast()->Init("Default",32);
			myOptionsText.GetLast()->SetText(temp.c_str());
			myOptionsText.GetLast()->SetPosition(CU::Vector2f(0.114f, 0.18f + 0.175f * row));
		
			temp = "Cost";
			temp += ": ";
			temp += std::to_string(mySelections[i]->myCost);
			myCostText.Add(new CTextInstance());
			myCostText.GetLast()->Init("Default",32);
			myCostText.GetLast()->SetText(temp.c_str());
			myCostText.GetLast()->SetPosition(CU::Vector2f(0.114f, 0.26f + 0.175f * row));
			++row;
		}
		else
		{
			std::string temp;
			temp += std::to_string(i + 1);
			temp += ". ";
			temp += mySelections[i]->HatName.c_str();
			myOptionsText.Add(new CTextInstance());
			myOptionsText.GetLast()->Init("Default", 32);
			myOptionsText.GetLast()->SetText(temp.c_str());
			myOptionsText.GetLast()->SetPosition(CU::Vector2f(0.027f, 0.18f + 0.175f * row));
		
			temp = "Cost";
			temp += ": ";
			temp += std::to_string(mySelections[i]->myCost);
			myCostText.Add(new CTextInstance());
			myCostText.GetLast()->Init("Default", 32);
			myCostText.GetLast()->SetText(temp.c_str());
			myCostText.GetLast()->SetPosition(CU::Vector2f(0.027f, 0.26f + 0.175f * row));
		}
	}
}
