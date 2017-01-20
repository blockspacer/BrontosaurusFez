#include "stdafx.h"
#include "HealthComponent.h"
#include "GameObject.h"
#include "..\PostMaster\PostMaster.h"
#include "..\PostMaster\EMessageReturn.h"
#include "..\PostMaster\Event.h"
#include "..\PostMaster\Message.h"

CHealthComponent::CHealthComponent()
{
	myMaxHealth = 100;
	myHealth = myMaxHealth;
	myPercentageLeft = static_cast<float>(myHealth) / static_cast<float>(myMaxHealth);

	PostMaster::GetInstance().Subscribe(this, eMessageType::eKeyPressed);
}


CHealthComponent::~CHealthComponent()
{
}

float CHealthComponent::GetPrecentLeft() const
{
	return myPercentageLeft;
}


void CHealthComponent::SetHealth(const HealthPoint aValue)
{
	bool wasAlreadyDead = false;
	if (myHealth <= 0) wasAlreadyDead = true;

	myHealth = aValue;
	if (myHealth > myMaxHealth)
	{
		myHealth = myMaxHealth;
	}
	myPercentageLeft = static_cast<float>(myHealth) / static_cast<float>(myMaxHealth);
	if (myHealth <= 0)
	{
		if (wasAlreadyDead == false)
		{
			//dead stuff
			GetParent()->NotifyComponents(eComponentMessageType::eDied, SComponentMessageData());
			SComponentMessageData data;
			data.myBool = false;
			GetParent()->NotifyComponents(eComponentMessageType::eSetVisibility, data);
		}
	}
}

void CHealthComponent::SetMaxHealth(const HealthPoint aValue)
{
	myMaxHealth = aValue;
	SetHealth(myMaxHealth * myPercentageLeft);
}

void CHealthComponent::Init()
{
	SComponentMessageData data;
	data.myComponent = this;
	GetParent()->NotifyComponents(eComponentMessageType::eSetMaxHealthFromStats,data);
}

void CHealthComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	SComponentMessageData data;
	switch (aMessageType)
	{
	case eComponentMessageType::eAddToMaxHealth:
		SetMaxHealth(myMaxHealth + aMessageData.myInt);
		break;
	case eComponentMessageType::eTakeDamage:
		SetHealth(myHealth - aMessageData.myInt);
		data.myUChar = myPercentageLeft * 100;
		GetParent()->NotifyComponents(eComponentMessageType::ePercentHPLeft, data);
		break;
	case eComponentMessageType::eHeal:
		SetHealth(myHealth + aMessageData.myInt);
		data.myUChar = myPercentageLeft * 100;
		GetParent()->NotifyComponents(eComponentMessageType::ePercentHPLeft, data);
		break;

	case eComponentMessageType::eRespawned:
		SetHealth(myMaxHealth);
		data.myUChar = myPercentageLeft * 100;
		GetParent()->NotifyComponents(eComponentMessageType::ePercentHPLeft, data);
		break;
	}
}

void CHealthComponent::Destroy()
{
}

eMessageReturn CHealthComponent::Recieve(const Message & aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}
