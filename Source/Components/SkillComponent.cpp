#include "stdafx.h"
#include "SkillComponent.h"


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
	if(myElapsedTime > 0.0f)
	{
		SComponentMessageData data2;
		data2.myBool = false;
		GetParent()->NotifyComponents(eComponentMessageType::eSetIsColliderActive, data2);
	}
	else
	{
		myElapsedTime += aDeltaTime;
	}
}

void SkillComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	SComponentMessageData data;
	SComponentMessageData data2;
	switch (aMessageType)
	{
	case eComponentMessageType::eCollision:
		data.myInt = myDamage;
		aMessageData.myGameObject->NotifyComponents(eComponentMessageType::eTakeDamage, data);
		data2.myBool = false;
		GetParent()->NotifyComponents(eComponentMessageType::eSetIsColliderActive, data2);
		break;
	case eComponentMessageType::eSetIsColliderActive:
		if(aMessageData.myBool == true)
		{
			myElapsedTime = 0.0f;
		}
		break;
	default:
		break;
	}
}

void SkillComponent::Destroy()
{
}
