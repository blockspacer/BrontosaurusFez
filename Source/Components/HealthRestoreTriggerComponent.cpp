#include "stdafx.h"
#include "HealthRestoreTriggerComponent.h"
#include "PollingStation.h"


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
			SComponentMessageData data;
			data.myInt = myHealAmount;
			PollingStation::playerObject->NotifyComponents(eComponentMessageType::eHealPercent, data);

			SComponentMessageData data2;
			data2.myString = "HealthWell";
			PollingStation::playerObject->NotifyComponents(eComponentMessageType::ePlaySound, data2);
		}
		break;
	}
}
