#include "stdafx.h"
#include "SkillSystemComponent.h"
#include "Skill.h"
#include "SkillFactory.h"

SkillSystemComponent::SkillSystemComponent()
{
	mySkills.Init(4);
	myType = eComponentType::eSkillSystem;
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
				eComponentMessageType type = eComponentMessageType::eAddSkill;
				SComponentMessageData data;
				data.myString = mySkills[i]->GetSkillData()->skillName.c_str();
				GetParent()->NotifyComponents(type, data);
			}

		}
	}
}

void SkillSystemComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	if(aMessageType == eComponentMessageType::eSelectSkill)
	{
		unsigned short index = -1;
		for (unsigned short i = 0; i < mySkills.Size(); i++)
		{
			if(mySkills[i] == aMessageData.mySkill)
			{
				index = i;
			}
		}
		if(index >= 0)
		{
			for(unsigned short i = 0; i < mySkills.Size(); i++)
			{
				mySkills[i]->Deselect();
				if(mySkills[index]->GetSkillData()->isChannel == true && i != index)
				{
					mySkills[i]->Deactivate();
				}
			}
			mySkills[index]->Select();
		}
		else
		{
			DL_PRINT_WARNING("Tried to use a skill that was not in skillSystemComponent");
		}
	
	}
	else if (aMessageType == eComponentMessageType::eTryToSelectSkill)
	{
		SComponentMessageData data;
		data.mySkill = mySkills[aMessageData.myInt];
		GetParent()->NotifyComponents(eComponentMessageType::eCheckIfCanSelect, data);
	}
	else if(aMessageType == eComponentMessageType::eSetSkillTargetPosition)
	{
		myTargetPosition = aMessageData.myVector3f;
		myTargetPosition.z = myTargetPosition.y;
		myTargetPosition.y = GetParent()->GetWorldPosition().y;
		for(unsigned short i = 0; i < mySkills.Size(); i++)
		{
			if(mySkills[i]->GetIsActive() == false)
			{
				mySkills[i]->SetTargetPosition(myTargetPosition);
				if (mySkills[i]->GetIsSelected() == true)
				{
					mySkills[i]->TryToActivate();
				}
			}
		}
	}
	else if (aMessageType == eComponentMessageType::eSetSkillTargetObject)
	{
		bool isAnythingSelected = false;
		bool isAnythingActive = false;
		for (unsigned short i = 0; i < mySkills.Size(); i++)
		{
			mySkills[i]->SetTargetObject(aMessageData.myGameObject);
			if (mySkills[i]->GetIsSelected() == true)
			{
				isAnythingSelected = true;
				mySkills[i]->TryToActivate();
			}
			if (mySkills[i]->GetIsActive() == true)
			{
				isAnythingActive = true;
			}
		}
		if(isAnythingSelected == false && isAnythingActive == false)
		{

			mySkills[0]->TryToActivate();
		}
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
				mySkills[i]->TryToActivate();
			}
		}
		if (isAnythingSelected == false)
		{
			mySkills[0]->TryToActivate();
		}
	}
	else if (aMessageType == eComponentMessageType::eDied)
	{
		myIsActive = false;
	}
	else if (aMessageType == eComponentMessageType::eRespawned)
	{
		myIsActive = true;
	}
	else if(aMessageType == eComponentMessageType::eStatsUpdated)
	{
		if (aMessageData.myStatStruct != nullptr)
		{
			for (int i = 0; i < mySkills.Size(); i++)
			{
				mySkills[i]->UpdateStats(*aMessageData.myStatStruct);
			}
		}
	}
	else if (aMessageType == eComponentMessageType::eActivateBurningBasicAttack)
	{
		for (int i = 0; i < mySkills.Size(); i++)
		{
			if(mySkills[i]->GetSkillData()->skillName == "BasicAttack")
			{
				mySkills[i]->GetSkillData()->damageBonus = 40.0f;
			}
		}
	}
	else if (aMessageType == eComponentMessageType::eActivateManaRefund)
	{
		for (int i = 0; i < mySkills.Size(); i++)
		{
			if (mySkills[i]->GetSkillData()->skillName == "SweepAttack")
			{
				mySkills[i]->GetSkillData()->manaRefund = 35.0f;
			}
		}
	}
	else if (aMessageType == eComponentMessageType::eStopWhirlwind)
	{
		for (int i = 0; i < mySkills.Size(); i++)
		{
			if (mySkills[i]->GetSkillData()->skillName == "WhirlWind")
			{
				if (mySkills[i]->GetIsActive() == true)
				{
					mySkills[i]->Deactivate();
				
				}
			}
		
		}
			
	}
}

void SkillSystemComponent::Destroy()
{
}

void SkillSystemComponent::AddSkill(const char* aSkillName)
{
	mySkills.Add(SkillFactory::GetInstance().CreateSkill(aSkillName));
	mySkills.GetLast()->SetTargetPosition(myTargetPosition);
}
