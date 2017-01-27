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
				
				GetParent()->NotifyComponents(eComponentMessageType::eHeal, messageData);
				//GetParent()->NotifyComponents(eComponentMessageType::eRestoreMana, messageData);
			}
			break;
		case ePickupType::MANA:
			{
				messageData.myInt = data.myValue;

				GetParent()->NotifyComponents(eComponentMessageType::eRestoreMana, messageData);
			}
			break;
		case ePickupType::GOLD:
		{

			PollingStation::playerData->myGold += data.myValue;
		}
		break;
		case ePickupType::HAT:
		{

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
}

void CPickerUpperComponent::Destroy()
{
	myManager.DestroyPickerUpperComp(this);
}

CPickerUpperComponent::CPickerUpperComponent(CPickupManager& aManager): myManager(aManager)
{
	myType = eComponentType::ePickerUpper;
}
