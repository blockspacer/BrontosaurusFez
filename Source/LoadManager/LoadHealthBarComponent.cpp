#include "stdafx.h"
#include "LoadHealthBarComponent.h"
#include "KevinLoader/KevinLoader.h"
#include "LoadManager.h"
#include "ComponentManager.h"
#include "HealthBarComponent.h"
#include "HealthBarComponentManager.h"

int LoadHealthBarComponent(KLoader::SLoadedComponentData someData)
{
	CHealthBarComponent* healthBar = LoadManager::GetInstance().GetCurrentPLaystate()->GetHealthBarManager()->CreateHealthbar();

	return healthBar->GetId();
}
