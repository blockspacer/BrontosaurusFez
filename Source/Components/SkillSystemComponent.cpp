#include "stdafx.h"
#include "SkillSystemComponent.h"
#include "Skill.h"
#include "SkillFactory.h"

#include <iostream>
SkillSystemComponent::SkillSystemComponent()
{
	mySkills.Init(4);
}


SkillSystemComponent::~SkillSystemComponent()
{
}

void SkillSystemComponent::Update(float aDeltaTime)
{
	for(unsigned short i = 0;  i < mySkills.Size(); i++)
	{
		if(mySkills[i]->GetIsActive() == true)
		{
			if(mySkills[i]->IsInited() == true)
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
			std::cout << "Tried to use a skill that was not in skillSystemComponent" << std::endl;
		}
	
	}
	else if(aMessageType == eComponentMessageType::eSetSkillTargetPosition)
	{
		myTargetPosition = aMessageData.myVector3f;
		for(unsigned short i = 0; i < mySkills.Size(); i++)
		{
			mySkills[i]->SetTarget(aMessageData.myVector3f);
			if(mySkills[i]->GetIsSelected() == true)
			{
				mySkills[i]->Activate();
			}
		}
	}
	else if (aMessageType == eComponentMessageType::eAddSkill)
	{
		mySkills.Add(SkillFactory::GetInstance().CreateSkill(aMessageData.myString));
		mySkills.GetLast()->Init(GetParent());
		mySkills.GetLast()->SetTarget(myTargetPosition);
	}
}

void SkillSystemComponent::Destroy()
{
}

void SkillSystemComponent::AddSkill(const char * aSkillName)
{
	mySkills.Add(SkillFactory::GetInstance().CreateSkill(aSkillName));
	mySkills.GetLast()->Init(GetParent());
	mySkills.GetLast()->SetTarget(myTargetPosition);
}
