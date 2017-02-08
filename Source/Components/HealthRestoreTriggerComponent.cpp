#include "stdafx.h"
#include "HealthRestoreTriggerComponent.h"
#include "PollingStation.h"
#include "../PostMaster/CoolBoi.h"

HealthRestoreTriggerComponent::HealthRestoreTriggerComponent()
{
	myCoolDown = 0;
	myHealAmount = 0;
	myElaspedTime = 0;
	myIsOnCoolDown = false;
}


HealthRestoreTriggerComponent::~HealthRestoreTriggerComponent()
{
}

void HealthRestoreTriggerComponent::Update(const float & aDeltaTime)
{
	if (myIsOnCoolDown == true)
	{
		myElaspedTime += aDeltaTime;
		if (myElaspedTime >= myCoolDown)
		{
			myIsOnCoolDown = false;
			myElaspedTime = 0;

			SComponentMessageData data;
			data.myBool = true;
			GetParent()->NotifyComponents(eComponentMessageType::eSetIsColliderActive, data);
		}
	}
}

void HealthRestoreTriggerComponent::SetHealAmount(const unsigned short& aAmount)
{
	myHealAmount = aAmount;
}

void HealthRestoreTriggerComponent::SetCoolDown(const float& aTime)
{
	myCoolDown = aTime;
}

void HealthRestoreTriggerComponent::Destroy()
{
}

void HealthRestoreTriggerComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	switch (aMessageType)
	{
	case(eComponentMessageType::eActivate):
		if (myIsOnCoolDown == false)
		{
			myIsOnCoolDown = true;
			SComponentMessageData healData;
			healData.myInt = myHealAmount;
			PollingStation::playerObject->NotifyComponents(eComponentMessageType::eHealPercent, healData);

			SComponentMessageData soundStrData;
			soundStrData.myString = "HealthWell";
			PollingStation::playerObject->NotifyComponents(eComponentMessageType::ePlaySound, soundStrData);

			SComponentMessageData turnOffPointLightData;
			GetParent()->NotifyComponents(eComponentMessageType::eTurnOffThePointLight, turnOffPointLightData);

			SComponentMessageData collisionActiveData;
			collisionActiveData.myBool = false;
			GetParent()->NotifyComponents(eComponentMessageType::eSetIsColliderActive, collisionActiveData);

			SComponentMessageData highlightData;
			highlightData.myFloat = 0.0f;
			GetParent()->NotifyComponents(eComponentMessageType::eSetHighlight, highlightData);

			PostMaster::GetInstance().SendLetter(Message(eMessageType::eShrineOrWellClicked, CoolBoi(GetParent())));
		}
		break;
	}
}
