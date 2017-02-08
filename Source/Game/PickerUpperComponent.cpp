#include "stdafx.h"
#include "PickerUpperComponent.h"
#include "PickupManager.h"
#include "PickupComponent.h"
#include "PostMaster/PostMaster.h"
#include "PostMaster/Message.h"
#include "../Components/GameObject.h"
#include "../Components/ComponentMessage.h"
#include "PollingStation.h"
#include "PlayerData.h"
#include "../PostMaster/PostMaster.h"
#include "../PostMaster/HatBought.h"
#include "ScriptComponentManager.h"

CPickerUpperComponent::~CPickerUpperComponent()
{
}

void CPickerUpperComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData)
{
	if (aMessageType == eComponentMessageType::ePickUp)
	{
		CPickupComponent* pickupComponent = aMessageData.myPickupComponent;
		const SPickupData data = CPickupManager::GetInstance().GetPickupData(pickupComponent);
		SComponentMessageData messageData;
		switch (data.myType)
		{
		case ePickupType::HEALTH:
			{
				
				messageData.myInt = data.myValue;
				
				GetParent()->NotifyComponents(eComponentMessageType::eHealPercent, messageData);

				SComponentMessageData messageData2;
				messageData2.myString = "HealthGlobe";
				GetParent()->NotifyComponents(eComponentMessageType::ePlaySound, messageData2);
				//GetParent()->NotifyComponents(eComponentMessageType::eRestoreMana, messageData);

				SComponentMessageData messageData3;
				messageData3.myString = "health";
				GetParent()->NotifyComponents(eComponentMessageType::ePickupVisual, messageData3);
			}
			break;
		case ePickupType::MANA:
			{
				messageData.myInt = data.myValue;

				GetParent()->NotifyComponents(eComponentMessageType::eRestorePercentMana, messageData);

				SComponentMessageData messageData2;
				messageData2.myString = "HealthGlobe";
				GetParent()->NotifyComponents(eComponentMessageType::ePlaySound, messageData2);

				SComponentMessageData messageData3;
				messageData3.myString = "mana";
				GetParent()->NotifyComponents(eComponentMessageType::ePickupVisual, messageData3);
			}
			break;
		case ePickupType::GOLD:
		{

			PollingStation::playerData->AddGold(data.myValue);

			SComponentMessageData messageData2;
			messageData2.myString = "PickupGold";
			GetParent()->NotifyComponents(eComponentMessageType::ePlaySound, messageData2);

			SComponentMessageData messageData3;
			messageData3.myString = "goldPickup";
			GetParent()->NotifyComponents(eComponentMessageType::ePickupVisual, messageData3);
		}
		break;
		case ePickupType::HAT:
		{

			SComponentMessageData messageData2;
			messageData2.myString = "AddedHat";
			GetParent()->NotifyComponents(eComponentMessageType::ePlaySound, messageData2);
			PostMaster::GetInstance().SendLetter(eMessageType::eHatAdded, HatBought(data.myString));
		}
		break;
		default: break;
		}

		SComponentMessageData messageData2;
		messageData2.myBool = false;
		pickupComponent->GetParent()->NotifyComponents(eComponentMessageType::eSetVisibility, messageData2);
		pickupComponent->GetParent()->NotifyComponents(eComponentMessageType::eSetIsColliderActive, messageData2);

	}
	else if (aMessageType == eComponentMessageType::eAddComponent)
	{
		if (aMessageData.myComponentTypeAdded == eComponentType::ePickerUpper)
		{
			CScriptComponentManager* scriptManager = CScriptComponentManager::GetInstance();
			if (scriptManager && GetParent())
			{
				GetParent()->AddComponent(scriptManager->CreateAbstractComponent("Script/pickup_emitter.lua"));
			}
		}
	}
}

void CPickerUpperComponent::Destroy()
{
	myManager.DestroyPickerUpperComp(this);
}

CPickerUpperComponent::CPickerUpperComponent(CPickupManager& aManager): myManager(aManager)
{
	myType = eComponentType::ePickerUpper;
}
