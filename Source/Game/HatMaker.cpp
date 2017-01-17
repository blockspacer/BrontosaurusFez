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

		/*blueprint->BaseStats.Strength = levelsArray[i].at("Strength").GetInt();
		blueprint->BaseStats.Dexterity = levelsArray[i].at("Dexterity").GetInt();
		blueprint->BaseStats.Intelligence = levelsArray[i].at("Intelligence").GetInt();
		blueprint->BaseStats.Vitality = levelsArray[i].at("Vitality").GetInt();

		blueprint->BonusStats.BonusCritChance = levelsArray[i].at("CritChance").GetFloat();
		blueprint->BonusStats.BonusCritDamage = levelsArray[i].at("CritDamage").GetFloat();
		blueprint->BonusStats.BonusMovementSpeed = levelsArray[i].at("MoveSpeed").GetFloat();
		blueprint->BonusStats.BonusArmor = levelsArray[i].at("Armor").GetFloat();
		blueprint->BonusStats.BonusDamage = levelsArray[i].at("Damage").GetFloat();
		blueprint->BonusStats.BonusHealth = levelsArray[i].at("Health").GetFloat();
		blueprint->BonusStats.BonusMana = levelsArray[i].at("Mana").GetFloat();*/

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
