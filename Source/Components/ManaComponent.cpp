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
	myType = eComponentType::eMana;
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
	SComponentMessageData data;
	data.myUChar = myPercentageLeft * 100;
	GetParent()->NotifyComponents(eComponentMessageType::ePercentMPLeft, data);
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
	GetParent()->NotifyComponents(eComponentMessageType::eSetMaxManaFromStats, data);
}

void ManaComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	SComponentMessageData data;
	switch (aMessageType)
	{
	case eComponentMessageType::eAddToMaxMana:
		SetMaxMana(myMaxMana + aMessageData.myInt);
		break;
	case eComponentMessageType::eBurnMana:
		SetMana(myMana - aMessageData.myInt);
		data.myUChar = myPercentageLeft * 100;
		GetParent()->NotifyComponents(eComponentMessageType::ePercentMPLeft, data);
		break;
	case eComponentMessageType::eCheckIfCanUseSkill:
		if (myMana >= static_cast<int>(aMessageData.mySkill->GetSkillData()->manaCost * aMessageData.mySkill->GetSkillData()->manaCostModifier))
		{
			data.myInt = static_cast<int>(aMessageData.mySkill->GetSkillData()->manaCost * aMessageData.mySkill->GetSkillData()->manaCostModifier);
			GetParent()->NotifyComponents(eComponentMessageType::eBurnMana, data);
			aMessageData.mySkill->Activate();
		}
		break;
	case eComponentMessageType::eRestoreMana:
		SetMana(myMana + aMessageData.myInt);
		data.myUChar = myPercentageLeft * 100;
		GetParent()->NotifyComponents(eComponentMessageType::ePercentMPLeft, data);
		break;
	case eComponentMessageType::eRestorePercentMana:
		float temp = myMaxMana * (aMessageData.myInt / 100);
		SetMana(myMana + temp);
		data.myUChar = myPercentageLeft * 100;
		GetParent()->NotifyComponents(eComponentMessageType::ePercentMPLeft, data);
		break;
	case eComponentMessageType::eRespawned:
		SetMana(myMaxMana);
		data.myUChar = myPercentageLeft * 100;
		GetParent()->NotifyComponents(eComponentMessageType::ePercentMPLeft, data);
		break;
	}
}

void ManaComponent::Destroy()
{
}