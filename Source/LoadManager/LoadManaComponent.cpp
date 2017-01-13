#include "stdafx.h"
#include "LoadManaComponent.h"
#include "KevinLoader/KevinLoader.h"
#include "LoadManager.h"
#include "ComponentManager.h"
#include "ManaComponent.h"
#include "ManaComponentManager.h"

int LoadManaComponent(KLoader::SLoadedComponentData someData)
{
	ManaComponent* Mana = ManaComponentManager::GetInstance().CreateAndRegisterComponent(someData.myData.at("Mana").GetInt());

	return Mana->GetId();
}
