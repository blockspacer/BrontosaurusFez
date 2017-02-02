#include "stdafx.h"
#include "LoadChangeLevelTriggerComponent.h"
#include "KevinLoader/KevinLoader.h"
#include "LoadManager.h"
#include "ComponentManager.h"
#include "../Components/ChangeLevelTriggerComponent.h"

int LoadChangeLevelTriggerComponent(KLoader::SLoadedComponentData someData)
{

	unsigned short id = 0;

	if (someData.myData.HasKey("ID") == true)
	{
		id = someData.myData.at("ID").GetUInt();
	}

	ChangeLevelTriggerComponent* levelChange = new ChangeLevelTriggerComponent(someData.myData.at("LevelToGoTo").GetUInt(), id);

	CComponentManager::GetInstance().RegisterComponent(levelChange);

	return levelChange->GetId();
}
