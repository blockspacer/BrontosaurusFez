#include "stdafx.h"
#include "HealthComponent.h"
#include "GameObject.h"
#include "../PostMaster/PostMaster.h"
#include "../PostMaster/EMessageReturn.h"
#include "../PostMaster/Event.h"
#include "../PostMaster/Message.h"
#include "../PostMaster/GameObjectDiedEvent.h"
#include "ModelInstance.h"
#include "ModelComponent.h"
#include "PollingStation.h"

std::string GetEmitterTypeFromObjectType(const eObjectType aObjectType);

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
			for (unsigned short i = 0; i < PollingStation::myThingsEnemiesShouldAvoid.Size(); i++)
			{
				if (PollingStation::myThingsEnemiesShouldAvoid[i]->GetId() == GetParent()->GetId())
				{
					PollingStation::myThingsEnemiesShouldAvoid.RemoveCyclicAtIndex(i);
					DL_PRINT("Removed enemy from things that should be avoided");
				}
			}
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
	SComponentMessageData soundData;
	float temp;
	switch (aMessageType)
	{
	case eComponentMessageType::eObjectDone:
	{
		CScriptComponentManager* scriptManager = CScriptComponentManager::GetInstance();
		if (!scriptManager) break;
		
		CComponent* bloodEmitter = scriptManager->CreateAbstractComponent(GetEmitterTypeFromObjectType(myObjectType));
		if (bloodEmitter)
		{
			GetParent()->AddComponent(bloodEmitter);
		}
	}
		break;
	case eComponentMessageType::eAddToMaxHealth:
		SetMaxHealth(myMaxHealth + aMessageData.myInt);
		break;
	case eComponentMessageType::eTakeDamage:
		SetHealth(myHealth - aMessageData.myInt);
		data.myUChar = myPercentageLeft * 100;
		GetParent()->NotifyComponents(eComponentMessageType::ePercentHPLeft, data); 
		//if (myObjectType != eObjectType::eBarrel && myObjectType != eObjectType::eUrn)
		{
			if (myObjectType == eObjectType::ePlayer)
			{
				soundData.myString = "Hit";
			}
			else
			{
				soundData.myString = "Slash";
			}
			GetParent()->NotifyComponents(eComponentMessageType::ePlaySound, soundData);
		}
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
	case eComponentMessageType::eQueryHealth:
		*static_cast<int*>(aMessageData.myVoidPointer) = myHealth;
		break;
	case eComponentMessageType::eDied:
		//switch ((eObjectType)((int)myObjectType + 1))
		switch (myObjectType)
		{
		case eObjectType::eUrn:
			DL_PRINT("an URN died");
			data.myString = "BreakUrn";
			GetParent()->NotifyComponents(eComponentMessageType::ePlaySound, data);
			data.myString = "Models/PlaceHolders/urnDead.fbx";
			GetParent()->NotifyComponents(eComponentMessageType::eChangeFBXToDead, data); // Anims handled another way?
			break;
		case eObjectType::eBarrel:
			DL_PRINT("a BARREL died");
			data.myString = "BreakBarrel";
			GetParent()->NotifyComponents(eComponentMessageType::ePlaySound, data);
			data.myString = "Models/PlaceHolders/barrelDead.fbx";
			GetParent()->NotifyComponents(eComponentMessageType::eChangeFBXToDead, data);
			break;
		case eObjectType::eChest:
			DL_PRINT("a CHEST died");
			data.myString = "BreakBarrel";
			GetParent()->NotifyComponents(eComponentMessageType::ePlaySound, data);
			data.myString = "Models/PlaceHolders/chestDead.fbx";
			GetParent()->NotifyComponents(eComponentMessageType::eChangeFBXToDead, data);
			break;
		case eObjectType::eWitch:
			DL_PRINT("a WITCH died");
			data.myString = "EnemyDie";
			GetParent()->NotifyComponents(eComponentMessageType::ePlaySound, data);
			break;
		case eObjectType::eZombie:
			data.myString = "EnemyDie";
			DL_PRINT("a ZOMBIE died");
			GetParent()->NotifyComponents(eComponentMessageType::ePlaySound, data);
			break;
		case eObjectType::eBlob:
			DL_PRINT("a BLOB died");
			data.myString = "EnemyDie";
			GetParent()->NotifyComponents(eComponentMessageType::ePlaySound, data);
			break;
		case eObjectType::eLeaper:
			DL_PRINT("a LEAPER died");
			data.myString = "EnemyDie";
			GetParent()->NotifyComponents(eComponentMessageType::ePlaySound, data);
			break;
		case eObjectType::ePlayer:
			DL_PRINT("a PLAYER died");
			data.myString = "EnemyDie";
			GetParent()->NotifyComponents(eComponentMessageType::ePlaySound, data);

			//data.myString = "bloodBath";
			//GetParent()->NotifyComponents(eComponentMessageType::eHealthVisual, data);
			break;


		case eObjectType::eDefault:
		default:
			DL_PRINT("a DEFAULT died"); ///yes I'm intentionally using DEFAULT as a noun.
			data.myString = "Models/Error/error.fbx";
			GetParent()->NotifyComponents(eComponentMessageType::eChangeFBXToDead, data);
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



std::string GetEmitterTypeFromObjectType(const eObjectType aObjectType)
{
	std::string name("");

	switch (aObjectType)
	{
	case eObjectType::eUrn:
	case eObjectType::eBarrel:
	case eObjectType::eWitch:
	case eObjectType::eChest:
		name = "Script/debris_emitter.lua";
		break;
	case eObjectType::eZombie:
	case eObjectType::eBlob:
	case eObjectType::ePlayer:
	case eObjectType::eLeaper:
		name = "Script/blood_emitter.lua";
		break;
	}

	return std::move(name);
}
