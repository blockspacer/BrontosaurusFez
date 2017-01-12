#include "stdafx.h"
#include "LoadHealthComponent.h"
#include "KevinLoader/KevinLoader.h"
#include "LoadManager.h"
#include "ComponentManager.h"
#include "HealthComponent.h"

int LoadHealthComponent(KLoader::SLoadedComponentData someData)
{
	CHealthComponent* health = new CHealthComponent();
	CComponentManager::GetInstance().RegisterComponent(health);
	health->SetMaxHealth(someData.myData.at("MaxHealth").GetUInt());
	return health->GetId();
}
