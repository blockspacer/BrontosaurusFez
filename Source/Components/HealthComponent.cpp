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
	myHealth = aValue;
	if (myHealth > myMaxHealth)
	{
		myHealth = myMaxHealth;
	}
	myPercentageLeft = static_cast<float>(myHealth) / static_cast<float>(myMaxHealth);
	if (myHealth <= 0)
	{
		//dead stuff
		GetParent()->NotifyComponents(eComponentMessageType::eDied, SComponentMessageData());
		SComponentMessageData data;
		data.myBool = false;
		GetParent()->NotifyComponents(eComponentMessageType::eSetVisibility, data);
	}
}

void CHealthComponent::SetMaxHealth(const HealthPoint aValue)
{
	myMaxHealth = aValue;
	myHealth = myMaxHealth * myPercentageLeft;
}

void CHealthComponent::Init()
{
	SComponentMessageData data;
	data.myComponent = this;
	GetParent()->NotifyComponents(eComponentMessageType::eSetMaxHealthFromStats,data);
}

void CHealthComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	switch (aMessageType)
	{
	case eComponentMessageType::eStatsUpdated:
		SetMaxHealth(aMessageData.myStatStruct.MaxHealth);
		break;
	case eComponentMessageType::eTakeDamage:
		SetHealth(myHealth - aMessageData.myInt);
		break;
	default:
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
