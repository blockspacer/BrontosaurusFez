#include "stdafx.h"
#include "LoadRespawnComponent.h"
#include "KevinLoader/KevinLoader.h"
#include "LoadManager.h"
#include "ComponentManager.h"
#include "RespawnComponent.h"
#include "RespawnComponentManager.h"

int LoadRespawnComponent(KLoader::SLoadedComponentData someData)
{
	RespawnComponent* respawn = RespawnComponentManager::GetInstance().CreateAndRegisterComponent();
	respawn->SetGoldLossPercentage(someData.myData.at("GoldLoss").GetFloat());
	respawn->SetRespawnTime(someData.myData.at("TimeBeforeRespawn").GetFloat());
	return respawn->GetId();
}
