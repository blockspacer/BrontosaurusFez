#include "stdafx.h"
#include "SkillComponent.h"
#include "../Collision/ICollider.h"
#include "../Game/PollingStation.h"

SkillComponent::SkillComponent(CGameObject* aUserObject)
{
	myDamage = 10000.0f;
	myIsAOE = false;
	myUserObject = aUserObject;
}


SkillComponent::~SkillComponent()
{
}

void SkillComponent::Update(float aDeltaTime)
{
	if(myIsActive == true)
	{
		if(myElapsedTime > 0.0f)
		{
			SComponentMessageData data2;
			data2.myBool = false;
			GetParent()->NotifyComponents(eComponentMessageType::eSetIsColliderActive, data2);
			myIsActive = false;
		}
		else
		{
			myElapsedTime += aDeltaTime;
		}
	}
}

void SkillComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	switch (aMessageType)
	{
	case eComponentMessageType::eOnCollisionEnter:
		if(myUserObject == PollingStation::playerObject)
		{
			if(aMessageData.myCollider->GetGameObject() != myUserObject)
			{
				DoDamage(aMessageData.myCollider->GetGameObject());
			}
		}
		else
		{
			if(aMessageData.myCollider->GetGameObject() == PollingStation::playerObject)
			{
				DoDamage(aMessageData.myCollider->GetGameObject());
			}
		}
		
		break;
	case eComponentMessageType::eSetIsColliderActive:
		if(aMessageData.myBool == true)
		{
			myElapsedTime = 0.0f;
			myIsActive = true;
		}
		break;
	default:
		break;
	}
}

void SkillComponent::Destroy()
{
}

void SkillComponent::DoDamage(CGameObject* aGameObjectToDamage)
{
	SComponentMessageData damageData;
	damageData.myInt = myDamage;
	aGameObjectToDamage->NotifyComponents(eComponentMessageType::eTakeDamage, damageData);

	if (myIsAOE == false)
	{
		SComponentMessageData deactivationData;
		deactivationData.myBool = false;
		GetParent()->NotifyComponents(eComponentMessageType::eSetIsColliderActive, deactivationData);
		myIsActive = false;
	}
}