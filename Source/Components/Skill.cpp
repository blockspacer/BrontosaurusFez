#include "stdafx.h"
#include "Skill.h"
#include "GameObject.h"


Skill::Skill()
{
	myIsActive = false;
	myIsSelected = false;
    myUpdateFunction = std::bind(&Skill::BasicAttackUpdate, this, std::placeholders::_1);
	myUser = nullptr;
}


Skill::~Skill()
{
}

void Skill::Activate()
{
	myIsActive = true;
	OnActivation();
}

void Skill::Deactivate()
{
	myIsActive = false;
	myIsSelected = false;
	OnDeActivation();
}

void Skill::Update(float aDeltaTime)
{
	myUpdateFunction(aDeltaTime);
}

void Skill::Init(CGameObject * aUser)
{
	myUser = aUser;
}

void Skill::BasicAttackUpdate(float aDeltaTime)
{
	float distance = CU::Vector3f(myUser->GetWorldPosition() - myTargetPosition).Length2();
	if(CU::Vector3f(myUser->GetWorldPosition() - myTargetPosition).Length2() < 50.0f * 50.0f)
	{
		eComponentMessageType type = eComponentMessageType::eStopMovement;
		myUser->NotifyComponents(type, SComponentMessageData());
		type = eComponentMessageType::eTakeDamage;
		SComponentMessageData data;
		data.myInt = 1000000000.0f;
		//myTarget->NotifyComponents(type, data); //from collsionMan get if there is an enemy here then do attack!
	}
}

void Skill::SetTarget(CU::Vector3f aTargetPosition)
{	
	myTargetPosition = aTargetPosition;
}

void Skill::OnActivation()
{

}

void Skill::OnDeActivation()
{
}
