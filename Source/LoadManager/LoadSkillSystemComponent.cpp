#include "stdafx.h"
#include "LoadSkillSystemComponent.h"
#include "KevinLoader/KevinLoader.h"
#include "LoadManager.h"
#include "ComponentManager.h"
#include "SkillSystemComponent.h"
#include "SkillSystemComponentManager.h"

int LoadSkillSystemComponent(KLoader::SLoadedComponentData someData)
{
	SkillSystemComponent* skillSystem = new SkillSystemComponent();
	SkillSystemComponentManager::GetInstance().RegisterComponent(skillSystem);
	//skillSystem->AddSkill(static_cast<SkillData::SkillName>(someData.myData.at("SkillName").GetUInt()));
	skillSystem->AddSkill(SkillData::SkillName::BasicAttack);
	skillSystem->AddSkill(SkillData::SkillName::SweepAttack);
	skillSystem->AddSkill(SkillData::SkillName::WhirlWind);
	CComponentManager::GetInstance().RegisterComponent(skillSystem);
	return skillSystem->GetId();
}
