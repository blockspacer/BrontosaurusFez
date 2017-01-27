#include "stdafx.h"
#include "LoadBlessingTowerComponent.h"
#include "KevinLoader/KevinLoader.h"
#include "LoadManager.h"
#include "ComponentManager.h"
#include "StatStructs.h"
#include "BlessingTowerComponent.h"
#include "BlessingTowerComponentManager.h"

int LoadBlessingTowerComponent(KLoader::SLoadedComponentData someData)
{
	BlessingTowerComponent* blessTower = BlessingTowerComponentManager::GetInstance().CreateAndRegisterComponent();
	blessTower->SetDamageModifier(someData.myData.at("DamageModifier").GetFloat());
	blessTower->SetDuration(someData.myData.at("Duration").GetFloat());
	blessTower->SetGiveManaAmount(someData.myData.at("ManaRestore").GetUInt());
	return blessTower->GetId();
}
