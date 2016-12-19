#include "stdafx.h"
#include "Skill.h"
#include "GameObject.h"


Skill::Skill()
{
    myUpdateFunction = std::bind(&Skill::BasicAttackUpdate, this, std::placeholders::_1);
	myUser = nullptr;
	myTarget = nullptr;
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
	if(CU::Vector3f(myUser->GetWorlPosition() - myTarget->GetWorlPosition()).Length2() < 500.0f * 500.0f)
	{
		eComponentMessageType type = eComponentMessageType::eTakeDamage;
		SComponentMessageData data;
		data.myInt = 1000000000.0f;
		myTarget->NotifyComponents(type, data);
	}
}

void Skill::OnActivation()
{

}

void Skill::OnDeActivation()
{
}
