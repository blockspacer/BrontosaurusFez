#include "stdafx.h"
#include "ManaComponent.h"
#include "GameObject.h"
#include "..\PostMaster\PostMaster.h"
#include "..\PostMaster\EMessageReturn.h"
#include "..\PostMaster\Event.h"
#include "..\PostMaster\Message.h"
#include "Skill.h"
#include "SkillData.h"

ManaComponent::ManaComponent(ManaPoint aMaxAmountOfMana)
{
	myMaxMana = aMaxAmountOfMana;
	myMana = myMaxMana;
	myPercentageLeft = static_cast<float>(myMana) / static_cast<float>(myMaxMana);
}


ManaComponent::~ManaComponent()
{
}

float ManaComponent::GetPrecentLeft() const
{
	return myPercentageLeft;
}


void ManaComponent::SetMana(const ManaPoint aValue)
{
	bool wasAlreadyDead = false;
	if (myMana <= 0) wasAlreadyDead = true;

	myMana = aValue;
	if (myMana > myMaxMana)
	{
		myMana = myMaxMana;
	}
	myPercentageLeft = static_cast<float>(myMana) / static_cast<float>(myMaxMana);
	if (myMana <= 0)
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

void ManaComponent::SetMaxMana(const ManaPoint aValue)
{
	myMaxMana = aValue;
	SetMana(myMaxMana * myPercentageLeft);
}

void ManaComponent::Init()
{
	SComponentMessageData data;
	data.myComponent = this;
	GetParent()->NotifyComponents(eComponentMessageType::eSetMaxHealthFromStats, data);
}

void ManaComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	SComponentMessageData data;
	switch (aMessageType)
	{
	case eComponentMessageType::eStatsUpdated:
		SetMaxMana(aMessageData.myStatStruct.MaxMana);
		break;
	case eComponentMessageType::eBurnMana:
		SetMana(myMana - aMessageData.myInt); 
		data.myUChar = myPercentageLeft * 100;
		GetParent()->NotifyComponents(eComponentMessageType::ePercentHPLeft, data);
		break;
	case eComponentMessageType::eCheckIfCanUseSkill:
		if (myMana >= aMessageData.mySkill->GetSkillData()->manaCost)
		{
			data.myInt = aMessageData.mySkill->GetSkillData()->manaCost;
			GetParent()->NotifyComponents(eComponentMessageType::eBurnMana, data);
			aMessageData.mySkill->Activate();
		}
		break;
	}
}

void ManaComponent::Destroy()
{
}