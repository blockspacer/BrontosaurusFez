#include "stdafx.h"
#include "SkillComponent.h"
//#include "../Collision/ICollider.h"
#include "../Game/PollingStation.h"
#include "SkillData.h"

SkillComponent::SkillComponent(CGameObject* aUserObject, SkillData* aSkillDataPointer)
{
	myUserObject = aUserObject;
	mySkillData = aSkillDataPointer;
	myType = eComponentType::eSkill;
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
			if(aMessageData.myGameObject != myUserObject)
			{
				DoDamage(aMessageData.myGameObject);
			}
		}
		else
		{
			if(aMessageData.myGameObject == PollingStation::playerObject)
			{
				DoDamage(aMessageData.myGameObject);
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
	damageData.myInt = static_cast<int>((mySkillData->damage + mySkillData->damageBonus) * mySkillData->damageModifier);
	aGameObjectToDamage->NotifyComponents(eComponentMessageType::eTakeDamage, damageData);

	if (mySkillData->isAOE == false)
	{
		SComponentMessageData deactivationData;
		deactivationData.myBool = false;
		GetParent()->NotifyComponents(eComponentMessageType::eSetIsColliderActive, deactivationData);
		myIsActive = false;
	}
}