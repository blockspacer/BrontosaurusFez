#include "stdafx.h"
#include "BlessingTowerComponent.h"
#include "../Game/PollingStation.h"
#include "StatStructs.h"
#include "../PostMaster/CoolBoi.h"
#include "../CommonUtilities/JsonValue.h"

BlessingTowerComponent::BlessingTowerComponent()
{
	myElapsedBuffTime = 0.0f;
	myIsActivated = false;
	myHaveBeenDeactivated = false;
	myDamageBonusStats = new Stats::SBonusStats;
}


BlessingTowerComponent::~BlessingTowerComponent()
{
}

void BlessingTowerComponent::Update(float aDeltaTime)
{
	if(myIsActivated == true)
	{
		myElapsedBuffTime += aDeltaTime;
		if(myElapsedBuffTime > myDuration)
		{
			if(myHaveBeenDeactivated == false)
			{
				myHaveBeenDeactivated = true;
				myDamageBonusStats->BonusDamageModifier *= -1;
				SComponentMessageData damaageData;

				damaageData.myStatsToAdd = myDamageBonusStats;
				PollingStation::playerObject->NotifyComponents(eComponentMessageType::eAddStats, damaageData);

				SComponentMessageData lightData;
				PollingStation::playerObject->NotifyComponents(eComponentMessageType::eSetPointLightToLastState, lightData);
			}
		}
	}
}

void BlessingTowerComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	if (aMessageType == eComponentMessageType::eActivate)
	{
		if (myIsActivated == false)
		{
			myIsActivated = true;

			SComponentMessageData soundData;
			soundData.myString = "Blessing";
			PollingStation::playerObject->NotifyComponents(eComponentMessageType::ePlaySound, soundData);
			
			SComponentMessageData manaData;
			manaData.myInt = myGiveManaAmount;
			PollingStation::playerObject->NotifyComponents(eComponentMessageType::eRestoreMana, manaData);

			SComponentMessageData damageData;
			damageData.myStatsToAdd = myDamageBonusStats;
			PollingStation::playerObject->NotifyComponents(eComponentMessageType::eAddStats, damageData);

			SComponentMessageData turnOffThePointLight;
			GetParent()->NotifyComponents(eComponentMessageType::eTurnOffThePointLight, turnOffThePointLight);

			SComponentMessageData colliderData;
			colliderData.myBool = false;
			GetParent()->NotifyComponents(eComponentMessageType::eSetIsColliderActive, colliderData);

			
			CU::CJsonValue levelsFile;

			std::string errorString = levelsFile.Parse("Json/BlessingTowerBuffLight.json");
			if (!errorString.empty()) DL_MESSAGE_BOX(errorString.c_str());

			CU::CJsonValue levelsArray = levelsFile.at("levels");

			CU::CJsonValue color = levelsArray.at("Color");

			SComponentMessageData lightColorData;
			lightColorData.myVector3f = color.GetVector3f("rgb"); //use this :D
			PollingStation::playerObject->NotifyComponents(eComponentMessageType::eSetPointLightColor, lightColorData);

			SComponentMessageData lightRangeData;
			lightRangeData.myFloat = levelsArray.at("Range").GetFloat();
			PollingStation::playerObject->NotifyComponents(eComponentMessageType::eSetPointLightRange, lightRangeData);

			SComponentMessageData lightIntensityData;
			lightIntensityData.myFloat = levelsArray.at("Intensity").GetFloat();
			PollingStation::playerObject->NotifyComponents(eComponentMessageType::eSetPointLightIntensity, lightIntensityData);


			PostMaster::GetInstance().SendLetter(Message(eMessageType::eShrineOrWellClicked, CoolBoi(GetParent())));
		}
	}
	
}

void BlessingTowerComponent::Destroy()
{
	SAFE_DELETE(myDamageBonusStats);
}
