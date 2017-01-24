#include "stdafx.h"
#include "ShopStorage.h"
#include "KevinLoader/KevinLoader.h"
#include "CommonUtilities/vector2.h"
#include "PostMaster/PostMaster.h"
#include "PostMaster/Message.h"
#include "PostMaster/Event.h"

CShopStorage* CShopStorage::ourInstance = nullptr;

void CShopStorage::Create()
{
	if (ourInstance == nullptr)
	{
		ourInstance = new CShopStorage();
	}
}

void CShopStorage::Destroy()
{
	if (ourInstance != nullptr)
	{
		delete ourInstance;
		ourInstance = nullptr;
	}
}

CShopStorage & CShopStorage::GetInstance()
{
	return *ourInstance;
}

void CShopStorage::RemoveHat(const std::string & aHatName)
{
	for (unsigned int i = 0; i < myStorage.HatStorage.Size(); ++i)
	{
		if (myStorage.HatStorage[i].HatName == aHatName)
		{
			myStorage.HatStorage.RemoveAtIndex(i);
			break;
		}
	}
	for (unsigned int i = 0; i < myStorage.StorageWithBuyOrder.Size(); ++i)
	{
		if (myStorage.StorageWithBuyOrder[i].Size() != 0)
		{
			if (myStorage.StorageWithBuyOrder[i][0].HatName == aHatName)
			{
				myStorage.StorageWithBuyOrder[i].RemoveAtIndex(0);
				break;
			}
		}
	}
}

void CShopStorage::LoadStorage(const std::string & aFilePath)
{
	CU::CJsonValue value;
	value.Parse(aFilePath);

	CU::CJsonValue hatsArray = value.at("Hats");
	for (int i = 0; i < hatsArray.Size(); ++i)
	{
		if (hatsArray[i].at("AvailableInShop").GetBool() == true)
		{
			SShopSelection* shopSelection = new SShopSelection();
			shopSelection->HatName = hatsArray[i].at("HatName").GetString();
			shopSelection->myCost = static_cast<SShopSelection::Cost>(hatsArray[i].at("ShopCost").GetInt());

			myStorage.HatStorage.Add(*shopSelection);
		}
	}
	SetupBuyOrder("Json/Hats/BuyOrder.json");
}

const CShopStorage::SMatchReturn CShopStorage::MatchHatWithName(const std::string & aHatName) const
{
	for (unsigned int i = 0; i < myStorage.HatStorage.Size(); ++i)
	{
		if (aHatName == myStorage.HatStorage[i].HatName)
		{
			CShopStorage::SMatchReturn returnData;
			returnData.Matched = true;
			returnData.MatchedWithThis = myStorage.HatStorage[i];
			return returnData;
		}
	}
	CShopStorage::SMatchReturn returnData;
	returnData.Matched = false;
	return returnData;
}

void CShopStorage::SetupBuyOrder(const std::string & aFilepath)
{
	CU::CJsonValue value;
	value.Parse(aFilepath);
	CU::CJsonValue BuyOrderArray = value.at("BuyOrder");
	if (BuyOrderArray.Size() != 0)
	{
		myStorage.StorageWithBuyOrder.Init(BuyOrderArray.Size());
		for (int i = 0; i < BuyOrderArray.Size(); ++i)
		{
			CU::CJsonValue HatArray = BuyOrderArray[i].at("HatArray");
			myStorage.StorageWithBuyOrder.Add(CU::GrowingArray<SShopSelection>());
			myStorage.StorageWithBuyOrder[i].Init(HatArray.Size());
			for (int j = 0; j < HatArray.Size(); ++j)
			{
				SMatchReturn matchdata = MatchHatWithName(HatArray[j].at("HatName").GetString());
				if (matchdata.Matched == true)
				{
					SShopSelection newSelection;
					newSelection.HatName = matchdata.MatchedWithThis.HatName;
					newSelection.myCost = matchdata.MatchedWithThis.myCost;
					myStorage.StorageWithBuyOrder[i].Add(newSelection);
				}
#ifndef _RETAIL_BUILD
				else
				{
					DL_ASSERT("Hat name did not match (%s)", HatArray[i].at("HatName").GetString());
				}
#endif // !_RETAIL_BUILD
			}
		}
	}
}

CShopStorage::CShopStorage()
{
	myStorage.HatStorage.Init(5);
	PostMaster::GetInstance().Subscribe(this, eMessageType::eHatAdded);
}


CShopStorage::~CShopStorage()
{
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eHatAdded);
}

eMessageReturn CShopStorage::Recieve(const Message & aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}
