#include "stdafx.h"
#include "LoadChangeLevelTriggerComponent.h"
#include "KevinLoader/KevinLoader.h"
#include "LoadManager.h"
#include "ComponentManager.h"
#include "../Components/ChangeLevelTriggerComponent.h"

int LoadChangeLevelTriggerComponent(KLoader::SLoadedComponentData someData)
{
	ChangeLevelTriggerComponent* levelChange = new ChangeLevelTriggerComponent(someData.myData.at("LevelToGoTo").GetUInt());

	CComponentManager::GetInstance().RegisterComponent(levelChange);

	return levelChange->GetId();
}
