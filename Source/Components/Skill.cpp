#include "stdafx.h"
#include "Skill.h"


Skill::Skill()
{
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
}

void Skill::OnActivation()
{

}

void Skill::OnDeActivation()
{
}
