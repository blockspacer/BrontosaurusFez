#include "stdafx.h"
#include "LoadMainStatComponent.h"
#include "KevinLoader/KevinLoader.h"
#include "LoadManager.h"
#include "ComponentManager.h"
#include "MainStatComponent.h"

int LoadMainStatComponent(KLoader::SLoadedComponentData someData)
{
	CMainStatComponent* mainstat = new CMainStatComponent();

	CComponentManager::GetInstance().RegisterComponent(mainstat);

	return mainstat->GetId();
}
