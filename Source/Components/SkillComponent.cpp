#include "stdafx.h"
#include "SkillComponent.h"
#include "../Collision/ICollider.h"

SkillComponent::SkillComponent()
{
	myDamage = 10000.0f;
	myIsAOE = false;
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
	SComponentMessageData data;
	SComponentMessageData data2;
	switch (aMessageType)
	{
	case eComponentMessageType::eOnCollisionEnter:
		data.myInt = myDamage;
		aMessageData.myCollider->GetGameObject()->NotifyComponents(eComponentMessageType::eTakeDamage, data);
		if(myIsAOE == false)
		{
			data2.myBool = false;
			GetParent()->NotifyComponents(eComponentMessageType::eSetIsColliderActive, data2);
			myIsActive = false;
		
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
