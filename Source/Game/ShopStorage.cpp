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
			myStorage.HatStorage.Remove(myStorage.HatStorage[i]);
			break;
		}
	}
}

void CShopStorage::LoadStorage(const std::string & aFilePath)
{
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

			myStorage.HatStorage.Add(*shopSelection);
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
