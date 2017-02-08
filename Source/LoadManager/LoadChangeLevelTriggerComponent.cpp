#include "stdafx.h"
#include "LoadChangeLevelTriggerComponent.h"
#include "KevinLoader/KevinLoader.h"
#include "LoadManager.h"
#include "ComponentManager.h"
#include "../Components/ChangeLevelTriggerComponent.h"
#include "ChangeLevelTriggerComponentManager.h"

int LoadChangeLevelTriggerComponent(KLoader::SLoadedComponentData someData)
{

	unsigned short id = 0;

	if (someData.myData.HasKey("ID") == true)
	{
		id = someData.myData.at("ID").GetUInt();
	}

	ChangeLevelTriggerComponent* levelChange = CChangeLevelTriggerComponentManager::GetInstance().CreateAndRegisterComponent(someData.myData.at("LevelToGoTo").GetUInt(), id);

	return levelChange->GetId();
}
