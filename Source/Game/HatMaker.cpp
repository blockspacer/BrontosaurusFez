#include "stdafx.h"
#include "HatMaker.h"
#include "PollingStation.h"
#include "PostMaster/Event.h"
#include "PostMaster/Message.h"
#include "PostMaster/PostMaster.h"
#include "Components/GameObject.h"
#include "Components/StatComponent.h"
#include "Components/ModelComponent.h"
#include "Components/GameObjectManager.h"
#include "Components/ModelComponentManager.h"
#include "KevinLoader/KevinLoader.h"
#include "Components/InventoryComponent.h"
#include "Components/HatActivator.h"
#include "Components/ComponentMessage.h"

CHatMaker::CHatMaker(CGameObjectManager* aGameObjectManager)
{
	PostMaster::GetInstance().Subscribe(this, eMessageType::eHatAdded);
	myGameObjectManager = aGameObjectManager;
}

CHatMaker::~CHatMaker()
{
	PostMaster::GetInstance().Subscribe(this, eMessageType::eHatAdded);
	myGameObjectManager = nullptr;
}

void CHatMaker::LoadBluePrints(const std::string& aFilePath)
{
	CU::CJsonValue HatBluePrint;
	const std::string& errorString = HatBluePrint.Parse(aFilePath);
	CU::CJsonValue levelsArray = HatBluePrint.at("Hats");

	for (unsigned int i = 0; i < levelsArray.Size(); ++i)
	{
		SHatBluePrint* blueprint = new SHatBluePrint();

		blueprint->HatName = levelsArray[i].at("HatName").GetString();
		blueprint->HatModel = levelsArray[i].at("Model").GetString();

		blueprint->myHatStruct = new SHat();
		blueprint->myHatStruct->stat = new Stats::SBonusStats;
		blueprint->myHatStruct->stat->BonusHealth = levelsArray[i].at("Health").GetFloat();
		blueprint->myHatStruct->stat->BonusMana = levelsArray[i].at("Mana").GetFloat();
		blueprint->myHatStruct->stat->BonusMovementSpeed = levelsArray[i].at("MoveSpeed").GetFloat();
		blueprint->myHatStruct->stat->BonusDamageModifier = levelsArray[i].at("DamageModifier").GetFloat();
		blueprint->myHatStruct->stat->BonusGoldGetModifier = levelsArray[i].at("GoldGetModifier").GetFloat();
		blueprint->myHatStruct->stat->BonusHealthDropChance = levelsArray[i].at("HealthDropChance").GetFloat();
		blueprint->myHatStruct->stat->BonusManaDropChance = levelsArray[i].at("ManaDropChance").GetFloat();
		blueprint->myHatStruct->stat->BonusManaCostModifier = levelsArray[i].at("ManaCostModifier").GetFloat();

		HatActivatorData* hatActivatorData = new HatActivatorData;
		float healthLimitActivator = levelsArray[i].at("HealthLimitActivation").GetFloat();
		float manaLimitActivator = levelsArray[i].at("ManaLimitActivation").GetFloat();
		healthLimitActivator /= 100.0f;
		manaLimitActivator /= 100.0f;
		hatActivatorData->healthLimitActivator = healthLimitActivator;
		hatActivatorData->manaLimitActivator = manaLimitActivator;
		hatActivatorData->isLimitActivationUnder = levelsArray[i].at("IsLimitActivatorUnder").GetBool();
		HatActivator* hatActivator;
		if(healthLimitActivator >= 1 && manaLimitActivator >= 1)
		{
			hatActivator = new HatActivator(true, hatActivatorData, blueprint->myHatStruct);
		}
		else
		{
			hatActivator = new HatActivator(false, hatActivatorData, blueprint->myHatStruct);
		}
		
		

		myBluePrints.emplace(blueprint->HatName, blueprint);
	}
}

void CHatMaker::MakeHatFromBluePrint(const std::string& aHatName)
{
	if (myBluePrints.find(aHatName) != myBluePrints.end())
	{
		SHatBluePrint* theBluePrint = myBluePrints.at(aHatName);
		CGameObject* hatObject = myGameObjectManager->CreateGameObject();
		CU::Vector3f hatPos = hatObject->GetLocalTransform().GetPosition();
		hatObject->GetLocalTransform().SetPosition({ hatPos.x, hatPos.y + 175.f, hatPos.z });
		
		CModelComponent* hatModel = CModelComponentManager::GetInstance().CreateComponent(theBluePrint->HatModel.c_str());
		hatObject->AddComponent(hatModel);
		theBluePrint->myHatStruct->gameObject = hatObject;

		if (PollingStation::playerObject != nullptr)
		{
			PollingStation::playerObject->AddComponent(hatObject);
			SComponentMessageData data;
			data.myHat = theBluePrint->myHatStruct;
			PollingStation::playerObject->NotifyComponents(eComponentMessageType::eAddHat, data);
		}


	}
}

eMessageReturn CHatMaker::Recieve(const Message & aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}
