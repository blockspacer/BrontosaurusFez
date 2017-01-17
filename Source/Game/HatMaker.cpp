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

		blueprint->BonusStats.BonusHealth = levelsArray[i].at("Health").GetFloat();
		blueprint->BonusStats.BonusMana = levelsArray[i].at("Mana").GetFloat();
		blueprint->BonusStats.BonusMovementSpeed = levelsArray[i].at("MoveSpeed").GetFloat();
		blueprint->BonusStats.BonusDamageModifier = levelsArray[i].at("DamageModifier").GetFloat();
		blueprint->BonusStats.BonusGoldGetModifier = levelsArray[i].at("GoldGetModifier").GetFloat();
		blueprint->BonusStats.BonusHealthDropChance = levelsArray[i].at("HealthDropChance").GetFloat();
		blueprint->BonusStats.BonusManaDropChance = levelsArray[i].at("ManaDropChance").GetFloat();
		blueprint->BonusStats.BonusManaCostModifier = levelsArray[i].at("ManaCostModifier").GetFloat();

		float healthLimitActivation = levelsArray[i].at("HealthLimitActivation").GetFloat();
		float manaLimitActivation = levelsArray[i].at("ManaLimitActivation").GetFloat();
		bool isLimitActivationUnder = levelsArray[i].at("IsLimitActivatorUnder").GetBool();
		

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
		CStatComponent* hatStats = new CStatComponent();
		hatObject->AddComponent(hatStats);
		hatStats->SetStats(theBluePrint->BaseStats, theBluePrint->BonusStats);
		CModelComponent* hatModel = CModelComponentManager::GetInstance().CreateComponent(theBluePrint->HatModel.c_str());
		hatObject->AddComponent(hatModel);

		if (PollingStation::playerObject != nullptr)
		{
			PollingStation::playerObject->AddComponent(hatObject);
		}
	}
}

eMessageReturn CHatMaker::Recieve(const Message & aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}
