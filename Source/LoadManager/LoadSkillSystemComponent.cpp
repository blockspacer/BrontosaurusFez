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

	if (someData.myData.HasKey("Skills") == false)
	{
		DL_PRINT("SkillSystemComponent empty got no skills added");

	}
	else
	{

	CU::CJsonValue levelsArray = someData.myData.at("Skills");

	if (!(levelsArray.IsArray() && levelsArray.Size() > 0))
	{
		DL_PRINT("SkillSystemComponent missing values");
	}

	for (unsigned short i = 0; i < levelsArray.Size(); i++)
	{
		skillSystem->AddSkill(levelsArray[i].GetString().c_str());
	}
	CComponentManager::GetInstance().RegisterComponent(skillSystem);
	return skillSystem->GetId();
	}

}
