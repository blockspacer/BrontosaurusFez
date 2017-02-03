#include "stdafx.h"
#include "HealthComponent.h"
#include "GameObject.h"
#include "../PostMaster/PostMaster.h"
#include "../PostMaster/EMessageReturn.h"
#include "../PostMaster/Event.h"
#include "../PostMaster/Message.h"
#include "../PostMaster/GameObjectDiedEvent.h"

CHealthComponent::CHealthComponent()
{
	myMaxHealth = 100;
	myHealth = myMaxHealth;
	myPercentageLeft = static_cast<float>(myHealth) / static_cast<float>(myMaxHealth);

	myType = eComponentType::eHealth;
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
			SComponentMessageData startedAttackingMessage;
			startedAttackingMessage.myString = "die";
			GetParent()->NotifyComponents(eComponentMessageType::eBasicAttack, startedAttackingMessage);
			/*SComponentMessageData data;
			data.myBool = false;
			GetParent()->NotifyComponents(eComponentMessageType::eSetVisibility, data);*/
			PostMaster::GetInstance().SendLetter(Message(eMessageType::eGameObjectDied, GameObjectDiedEvent(GetParent())));
		}
	}
}

void CHealthComponent::SetMaxHealth(const HealthPoint aValue)
{
	myMaxHealth = aValue;
	SetHealth(myMaxHealth * myPercentageLeft);
}

void CHealthComponent::SetObjectType(const eObjectType aType)
{
	myObjectType = aType;
}

#include "ScriptComponentManager.h"

void CHealthComponent::Init()
{
	SComponentMessageData data;
	data.myComponent = this;
	GetParent()->NotifyComponents(eComponentMessageType::eSetMaxHealthFromStats,data);
}

void CHealthComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	SComponentMessageData data;
	float temp;
	switch (aMessageType)
	{
	case eComponentMessageType::eObjectDone:
	{
		CScriptComponentManager* scriptManager = CScriptComponentManager::GetInstance();
		if (!scriptManager) break;
		CComponent* bloodEmitter = scriptManager->CreateAbstractComponent("Script/blood_emitter.lua");
		GetParent()->AddComponent(bloodEmitter);
	}
		break;
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
	case eComponentMessageType::eHealPercent:
		temp = static_cast<float>(myMaxHealth * (static_cast<float>(aMessageData.myInt / 100.0f)));
		SetHealth(myHealth + temp);
		data.myUChar = myPercentageLeft * 100;
		GetParent()->NotifyComponents(eComponentMessageType::ePercentHPLeft, data);
		break;
	case eComponentMessageType::eRespawned:
		SetHealth(myMaxHealth);
		data.myUChar = myPercentageLeft * 100;
		GetParent()->NotifyComponents(eComponentMessageType::ePercentHPLeft, data);
		break;
	case eComponentMessageType::eDied:
		switch (myObjectType)
		{
		case eObjectType::eUrn:
			data.myString = "BreakUrn";
			GetParent()->NotifyComponents(eComponentMessageType::ePlaySound, data);
			break;
		case eObjectType::eBarrel:
			data.myString = "BreakBarrel";
			GetParent()->NotifyComponents(eComponentMessageType::ePlaySound, data);
			break;
		case eObjectType::eWitch:
			data.myString = "EnemyDie";
			GetParent()->NotifyComponents(eComponentMessageType::ePlaySound, data);
			break;
		case eObjectType::eZombie:
			data.myString = "EnemyDie";
			GetParent()->NotifyComponents(eComponentMessageType::ePlaySound, data);
			break;
		case eObjectType::eBlob:
			data.myString = "EnemyDie";
			GetParent()->NotifyComponents(eComponentMessageType::ePlaySound, data);
			break;
		case eObjectType::ePlayer:
			data.myString = "EnemyDie";
			GetParent()->NotifyComponents(eComponentMessageType::ePlaySound, data);
			break;
		default:
			break;
		}
	}
}

void CHealthComponent::Destroy()
{
}

eMessageReturn CHealthComponent::Recieve(const Message & aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}
