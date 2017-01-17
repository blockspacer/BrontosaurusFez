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

	CU::CJsonValue levelsArray = someData.myData.at("Skills");

	for (unsigned short i = 0; i < levelsArray.Size(); i++)
	{
		skillSystem->AddSkill(levelsArray[i].GetString().c_str());
	}
	CComponentManager::GetInstance().RegisterComponent(skillSystem);
	return skillSystem->GetId();
}
