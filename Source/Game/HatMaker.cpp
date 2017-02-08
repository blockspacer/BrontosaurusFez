#include "stdafx.h"
#include "..\BrontosaurusEngine\Engine.h"
#include "..\BrontosaurusEngine\ModelManager.h"
#include "..\BrontosaurusEngine\ModelInstance.h"

#include "..\CommonUtilities\StringHelper.h"

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
#include "Component.h"
#include "PostMaster/PushState.h"



namespace
{
#ifndef RAND_FLOAT_RANGE
#define RAND_FLOAT_RANGE(LOW, HIGH) (LOW) + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/((HIGH)-(LOW))));
#endif
}





CHatMaker::CHatMaker(CGameObjectManager* aGameObjectManager)
{
	PostMaster::GetInstance().Subscribe(this, eMessageType::eHatAdded);
	myHatObjects.Init(14);

	myGameObjectManager = aGameObjectManager;
	myHatsGivenToPlayer = 0;
}

CHatMaker::~CHatMaker()
{
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eHatAdded);
	myGameObjectManager = nullptr;
	
	
	
	//TODO: Kanske minnesläcka :O
	myHatObjects.RemoveAll();
}

void CHatMaker::Update()
{
	if (myHatObjects.Size() > 0)
	{
		CModel* model = CEngine::GetInstance()->GetModelManager()->GetModel(myPlayerModel->GetModelInst()->GetModelID());


		std::string boneName = "hatslot";
		boneName += std::to_string(/*i +*/ 1);
		boneName += "_SKIN";
		CU::Matrix44f hatTransform = model->GetBoneTransform(myPlayerModel->GetModelInst()->GetAnimationCounter(), myPlayerModel->GetModelInst()->GetAnimationState(), boneName.c_str());
		float startHeight = hatTransform.m42;

		for (int i = 0; i < myHatObjects.Size(); ++i)
		{
			hatTransform.m42 = startHeight + 10.f  * i;
			myHatObjects[i].myObjectPtr->GetLocalTransform() = myHatObjects[i].myTransformation * hatTransform;
		}
		PollingStation::playerObject->NotifyComponents(eComponentMessageType::eMoving, SComponentMessageData());
	}
}


void CHatMaker::LoadBluePrints(const std::string& aFilePath)
{
	CU::CJsonValue HatBluePrint;
	const std::string& errorString = HatBluePrint.Parse(aFilePath);
	CU::CJsonValue levelsArray = HatBluePrint.at("Hats");

	for (int i = 0; i < levelsArray.Size(); ++i)
	{
		SHatBluePrint* blueprint = new SHatBluePrint();

		blueprint->HatName = levelsArray[i].at("HatName").GetString();
		blueprint->HatModel = levelsArray[i].at("Model").GetString();
		blueprint->HatDialog = "";

		if (levelsArray[i].Count("Dialog") > 0)
		{
			blueprint->HatDialog = levelsArray[i].at("Dialog").GetString();
		}

		blueprint->myHatStruct = new SHat();
		blueprint->myHatStruct->stat = new Stats::SBonusStats;
		float BonusDamageModifier = levelsArray[i].at("DamageModifier").GetFloat();
		BonusDamageModifier /= 100.0f;
		float BonusGoldGetModifier = levelsArray[i].at("GoldGetModifier").GetFloat();
		BonusGoldGetModifier /= 100.0f;
		float BonusManaCostModifier = levelsArray[i].at("ManaCostModifier").GetFloat();
		BonusManaCostModifier /= 100.0f;
		blueprint->myHatStruct->stat->BonusHealth = levelsArray[i].at("Health").GetFloat();
		blueprint->myHatStruct->stat->BonusMana = levelsArray[i].at("Mana").GetFloat();
		blueprint->myHatStruct->stat->BonusMovementSpeed = levelsArray[i].at("MoveSpeed").GetFloat();
		blueprint->myHatStruct->stat->BonusDamageModifier = BonusDamageModifier;
		blueprint->myHatStruct->stat->BonusGoldGetModifier = BonusGoldGetModifier;
		blueprint->myHatStruct->stat->BonusHealthDropChance = levelsArray[i].at("HealthDropChance").GetFloat();
		blueprint->myHatStruct->stat->BonusManaDropChance = levelsArray[i].at("ManaDropChance").GetFloat();
		blueprint->myHatStruct->stat->BonusManaCostModifier = BonusManaCostModifier;
		blueprint->myHatStruct->skillname = levelsArray[i].at("Skill").GetString().c_str();

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
		
		blueprint->myHatStruct->hatActivator = hatActivator;

		myBluePrints.emplace(blueprint->HatName, blueprint);
	}

	CU::GrowingArray<CComponent*> playerComponents = PollingStation::playerObject->GetComponents();

	myPlayerModel = nullptr;
	for (unsigned int i = 0; i < playerComponents.Size(); ++i)
	{
		if (playerComponents[i]->GetType() == eComponentType::eModel)
		{
			myPlayerModel = static_cast<CModelComponent*>(playerComponents[i]);
			break;
		}
	}
	assert(myPlayerModel != nullptr && "Didn't find PlayerModel");
}

void CHatMaker::MakeHatFromBluePrint(const std::string& aHatName,const bool aIsInBeginningOfLevel)
{
	if (myBluePrints.find(aHatName) != myBluePrints.end())
	{
		SHatBluePrint* theBluePrint = myBluePrints.at(aHatName);
		CGameObject* hatObject = myGameObjectManager->CreateGameObject();
		
		SHatObject cashedHat;
		cashedHat.myObjectPtr = hatObject;
		//Hat offset magics

		CU::Vector3f hatPositionOffset;

		if (myHatObjects.Size() > 0) hatPositionOffset = myHatObjects.GetLast().myTransformation.GetPosition();

		int i = min(myHatObjects.Size(), 1);
		hatPositionOffset.x += RAND_FLOAT_RANGE(-5.f, 5.f);
		hatPositionOffset.y += RAND_FLOAT_RANGE(0.f, 5.f);
		hatPositionOffset.z += RAND_FLOAT_RANGE(-5.f, 5.f);
		hatPositionOffset *= i;
		cashedHat.myTransformation.SetPosition(hatPositionOffset);
		float scale = RAND_FLOAT_RANGE(0.75f, 1.25f);
		cashedHat.myTransformation.Scale({ scale, scale, scale });


		
		float angle = PI / 16.f;
		float x = RAND_FLOAT_RANGE(-angle, angle);
		float y = RAND_FLOAT_RANGE(0, 2 * PI);
		float z = RAND_FLOAT_RANGE(-angle, angle);

		cashedHat.myTransformation.Rotate(x, y, z);
		//end of life and all		
		






		myHatObjects.Add(cashedHat);
		CU::Vector3f hatPos = hatObject->GetLocalTransform().GetPosition();
		//hatObject->GetLocalTransform().SetPosition({ hatPos.x, hatPos.y + 175.f + 12.5f * myHatsGivenToPlayer, hatPos.z });
		myHatsGivenToPlayer++;
		CModelComponent* hatModel = CModelComponentManager::GetInstance().CreateComponent(theBluePrint->HatModel.c_str());
		hatObject->AddComponent(hatModel);
		theBluePrint->myHatStruct->gameObject = hatObject;

		bool wasfound = false;
		for (unsigned int i = 0; i < PollingStation::playerHatList.Size(); ++i)
		{
			if (aHatName == PollingStation::playerHatList[i])
			{
				wasfound = true;
				break;
			}
		}
		if (wasfound == false)
		{
			std::string temp = aHatName;
			PollingStation::playerHatList.Add(temp);
		}

		if (PollingStation::playerObject != nullptr)
		{
			PollingStation::playerObject->AddComponent(hatObject);
			SComponentMessageData data;
			data.myHat = theBluePrint->myHatStruct;
			PollingStation::playerObject->NotifyComponents(eComponentMessageType::eAddHat, data);
		}
		if(theBluePrint->HatName == "FireHat")
		{
			PollingStation::playerObject->NotifyComponents(eComponentMessageType::eActivateBurningBasicAttack, SComponentMessageData());
		}
		else if (theBluePrint->HatName == "SweepAttackHatLvl2")
		{
			PollingStation::playerObject->NotifyComponents(eComponentMessageType::eActivateManaRefund, SComponentMessageData());
		}

		if (aIsInBeginningOfLevel == false && theBluePrint->HatDialog != "")
		{
			PollingStation::currentDialog = theBluePrint->HatDialog;
			PostMaster::GetInstance().SendLetter(eMessageType::eStateStackMessage, PushState(PushState::eState::eDialog, 1));
		}
	}
	SComponentMessageData data;
	PollingStation::playerObject->NotifyComponents(eComponentMessageType::eMoving, data);
}

void CHatMaker::GiveTheManAHat()
{
	for (unsigned int i = 0; i < PollingStation::playerHatList.Size(); ++i)
	{
		MakeHatFromBluePrint(PollingStation::playerHatList[i]);
	}
	SComponentMessageData data;
	PollingStation::playerObject->NotifyComponents(eComponentMessageType::eMoving, data);
}

const bool CHatMaker::CheckIfHatIsMade(const std::string & aHatName)
{
	return false;
}


eMessageReturn CHatMaker::Recieve(const Message & aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}
