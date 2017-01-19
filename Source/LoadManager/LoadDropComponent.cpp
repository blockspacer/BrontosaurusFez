#include "stdafx.h"
#include "LoadDropComponent.h"
#include "KevinLoader/KevinLoader.h"
#include "LoadManager.h"
#include "ComponentManager.h"
#include "DropComponent.h"
#include "DropComponentManager.h"

int LoadDropComponent(KLoader::SLoadedComponentData someData)
{

	int temp = (rand() % someData.myData.at("MaxGoldToDropOnDeath").GetUInt()) + someData.myData.at("MinGoldToDropOnDeath").GetUInt();

	short chance = someData.myData.at("PercentChanceToDropGlobe").GetUInt();

	DropComponent* dropComp = DropComponentManager::GetInstance().CreateAndRegisterComponent(temp, chance);

	

	return dropComp->GetId();
}
