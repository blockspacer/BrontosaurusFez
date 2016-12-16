#include "stdafx.h"
#include "HealthComponent.h"
#include "GameObject.h"

CHealthComponent::CHealthComponent()
{
	myMaxHealth = 100;
	myHealth = myMaxHealth;
	myPercentageLeft = static_cast<float>(myHealth) / static_cast<float>(myMaxHealth);

	SComponentMessageData data;
	data.myComponent = this;
	GetParent()->NotifyComponents(eComponentMessageType::eSetMaxHealthFromStats,data);
}


CHealthComponent::~CHealthComponent()
{
}

const float CHealthComponent::GetPrecentLeft()
{
	return myPercentageLeft;
}

void CHealthComponent::Update(const CU::Time & aDeltaTime)
{
		
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
	}
}

void CHealthComponent::SetMaxHealth(const HealthPoint aValue)
{
	myMaxHealth = aValue;
	myHealth = myMaxHealth * myPercentageLeft;
}

void CHealthComponent::Destroy()
{
}
