#include "stdafx.h"
#include "SkillSystemComponent.h"
#include "Skill.h"

#include <iostream>
SkillSystemComponent::SkillSystemComponent()
{
	mySkills.Init(4);
	myTarget = nullptr;
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
			mySkills[i]->Update(aDeltaTime);

		}
	}
}

void SkillSystemComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	if(aMessageType == eComponentMessageType::eUseSkill)
	{
		if(aMessageData.myInt < mySkills.Size())
		{
			mySkills[aMessageData.myInt]->Activate();
		}
		else
		{
			std::cout << "Tried to use a skill that was not in skillSystemComponent" << std::endl;
		}
	
	}
	else if(aMessageType == eComponentMessageType::eSetSkillTarget)
	{
		myTarget = aMessageData.myGameObject;
	}
	else if (aMessageType == eComponentMessageType::eAddSkill)
	{
		myTarget = aMessageData.myGameObject;
	}
}

void SkillSystemComponent::Destroy()
{
}
