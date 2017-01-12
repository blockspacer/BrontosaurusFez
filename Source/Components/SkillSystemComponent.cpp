#include "stdafx.h"
#include "SkillSystemComponent.h"
#include "Skill.h"
#include "SkillFactory.h"

SkillSystemComponent::SkillSystemComponent()
{
	mySkills.Init(4);
	myIsActive = true;
}


SkillSystemComponent::~SkillSystemComponent()
{
}

void SkillSystemComponent::Update(float aDeltaTime)
{
	if(myIsActive == true)
	{
		for (unsigned short i = 0; i < mySkills.Size(); i++)
		{
			if (mySkills[i]->IsInited() == true)
			{
				mySkills[i]->Update(aDeltaTime);

			}
			else
			{
				mySkills[i]->Init(GetParent());
				mySkills[i]->Update(aDeltaTime);
			}

		}
	}
}

void SkillSystemComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	if(aMessageType == eComponentMessageType::eSelectSkill)
	{
		if(aMessageData.myInt < mySkills.Size())
		{
			for(unsigned short i = 0; i < mySkills.Size(); i++)
			{
				mySkills[i]->Deselect();
			}
			mySkills[aMessageData.myInt]->Select();
		}
		else
		{
			DL_PRINT_WARNING("Tried to use a skill that was not in skillSystemComponent");
		}
	
	}
	else if(aMessageType == eComponentMessageType::eSetSkillTargetPosition)
	{
		myTargetPosition = aMessageData.myVector3f;
		myTargetPosition.z = myTargetPosition.y;
		myTargetPosition.y = GetParent()->GetWorldPosition().y;
		for(unsigned short i = 0; i < mySkills.Size(); i++)
		{
			mySkills[i]->SetTargetPosition(myTargetPosition);
			if(mySkills[i]->GetIsSelected() == true)
			{
				mySkills[i]->Activate();
			}
		}
	}
	else if (aMessageType == eComponentMessageType::eSetSkillTargetObject)
	{
		bool isAnythingSelected = false;
		for (unsigned short i = 0; i < mySkills.Size(); i++)
		{
			mySkills[i]->SetTargetObject(aMessageData.myGameObject);
			if (mySkills[i]->GetIsSelected() == true)
			{
				isAnythingSelected = true;
				mySkills[i]->Activate();
			}
		}
		if(isAnythingSelected == false)
		{
			mySkills[0]->Activate();
		}
	}
	else if (aMessageType == eComponentMessageType::eAddSkill)
	{
		mySkills.Add(SkillFactory::GetInstance().CreateSkill(aMessageData.myString));
		mySkills.GetLast()->Init(GetParent());
		mySkills.GetLast()->SetTargetPosition(myTargetPosition);
	}
	else if (aMessageType == eComponentMessageType::eActivateSkillCollider)
	{
		for(unsigned short i = 0; i < mySkills.Size(); i++)
		{
			if(mySkills[i]->GetIsActive() == true)
			{
				mySkills[i]->ActivateCollider();
			}
		}
	}
	else if(aMessageType == eComponentMessageType::eSetSkillTargetPositionWhileHoldingPosition)
	{
		bool isAnythingSelected = false;
		myTargetPosition = aMessageData.myVector3f;
		myTargetPosition.z = myTargetPosition.y;
		myTargetPosition.y = GetParent()->GetWorldPosition().y;
		for (unsigned short i = 0; i < mySkills.Size(); i++)
		{
			mySkills[i]->SetTargetPosition(myTargetPosition);
			if (mySkills[i]->GetIsSelected() == true)
			{
				mySkills[i]->Activate();
			}
		}
		if (isAnythingSelected == false)
		{
			mySkills[0]->Activate();
		}
	}
	else if (aMessageType == eComponentMessageType::eDied)
	{
		myIsActive = false;
	}
}

void SkillSystemComponent::Destroy()
{
}

void SkillSystemComponent::AddSkill(const char * aSkillName)
{
	eComponentMessageType type = eComponentMessageType::eAddSkill;
	SComponentMessageData data;
	data.myString = aSkillName;
	GetParent()->NotifyComponents(type, data);
	/*mySkills.Add(SkillFactory::GetInstance().CreateSkill(aSkillName));
	mySkills.GetLast()->Init(GetParent());
	mySkills.GetLast()->SetTarget(myTargetPosition);*/
}
