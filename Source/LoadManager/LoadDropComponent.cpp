#include "stdafx.h"
#include "LoadDropComponent.h"
#include "KevinLoader/KevinLoader.h"
#include "LoadManager.h"
#include "ComponentManager.h"
#include "DropComponent.h"
#include "DropComponentManager.h"
#include <string>

int LoadDropComponent(KLoader::SLoadedComponentData someData)
{
	int temp1 = someData.myData.at("MaxGoldToDropOnDeath").GetUInt();
	int temp2 = someData.myData.at("MinGoldToDropOnDeath").GetUInt();

	int temp = (rand() % temp1) + temp2;

	short healthChance = 0;
	short manaChance = 0;
	std::string hatDrop = "";
	DropComponent* dropComp;
	if (someData.myData.HasKey("PercentChanceToDropHealthGlobe") == true)
	{
		healthChance = someData.myData.at("PercentChanceToDropHealthGlobe").GetUInt();
	}

	if (someData.myData.HasKey("PercentChanceToDropManaGlobe") == true)
	{
		manaChance = someData.myData.at("PercentChanceToDropManaGlobe").GetUInt();
	}

	if (someData.myData.HasKey("HatDrop") == true)
	{
		dropComp = DropComponentManager::GetInstance().CreateAndRegisterComponent(temp, healthChance, manaChance, someData.myData.at("HatDrop").GetString().c_str());
	}
	else
	{
		dropComp = DropComponentManager::GetInstance().CreateAndRegisterComponent(temp, healthChance, manaChance, hatDrop.c_str());

	}


	

	return dropComp->GetId();
}
