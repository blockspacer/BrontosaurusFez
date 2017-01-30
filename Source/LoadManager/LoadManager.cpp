#include "stdafx.h"
#include "LoadManager.h"
#include "DL_Debug.h"
#include "LoadObject.h"
#include "KevinLoader/KevinLoader.h"
#include "LoadModel.h"
#include "LoadInputController.h"
#include "LoadNavigationComponent.h"
#include "LoadMovementComponent.h"
#include "LoadCamera.h"
#include "LoadColliderComponent.h"
#include "LoadAIController.h"
#include "LoadSkillSystemComponent.h"
#include "LoadHealthComponent.h"
#include "LoadHealthBarComponent.h"
#include "LoadDropComponent.h"
#include "LoadManaComponent.h"
#include "LoadSoundComponent.h"
#include "LoadMainStatComponent.h"
#include "LoadInventoryComponent.h"
#include "LoadPickerupperComponent.h"
#include "LoadObjectives.h"
#include "LoadActivationComponent.h"
#include "LoadOpenShopListenerComponent.h"
#include "LoadChangeLevelTriggerComponent.h"
#include "LoadBlessingTowerComponent.h"
#include "LoadSpawnEnemyOnDeathComponent.h"
#include "LoadRespawnComponent.h"

LoadManager* LoadManager::ourInstance = nullptr;

void LoadManager::CreateInstance()
{
	assert(ourInstance == nullptr && "Load manager already created");
	ourInstance = new LoadManager;
}

LoadManager& LoadManager::GetInstance()
{
	if (ourInstance == nullptr)
	{
		DL_ASSERT("Trying to get load manager but load manager isn't created");
	}

	return *ourInstance;
}

void LoadManager::DestroyInstance()
{
	assert(ourInstance != nullptr && "Load manager not created (is NULL)");
	SAFE_DELETE(ourInstance);
}

void LoadManager::SetCurrentScene(CScene* aScene)
{
	myCurrentScene = aScene;
}

CScene* LoadManager::GetCurrentScene() const
{
	return myCurrentScene;
}

void LoadManager::SetCurrentPlayState(CPlayState* aPlaystate)
{
	myCurrentPlaystate = aPlaystate;
}

CPlayState* LoadManager::GetCurrentPLaystate() const
{
	return myCurrentPlaystate;
}

void LoadManager::RegisterFunctions()
{
	KLoader::CKevinLoader &loader = KLoader::CKevinLoader::GetInstance();

	loader.RegisterObjectLoadFunction(LoadObject);
	loader.RegisterObjectLinkFunction(LinkObject);

	loader.RegisterComponentLoadFunction("MeshFilter", LoadMeshFilter);
	loader.RegisterComponentLoadFunction("InputController", LoadInputController);
	loader.RegisterComponentLoadFunction("Navigation", LoadNavigationComponent);
	loader.RegisterComponentLoadFunction("Movement", LoadMovementComponent);
	loader.RegisterComponentLoadFunction("Camera", LoadCamera);
	loader.RegisterComponentLoadFunction("CircleCollider", LoadCircleCollider);
	loader.RegisterComponentLoadFunction("AIController", LoadAIController);
	loader.RegisterComponentLoadFunction("SeekController", LoadSeekController);
	loader.RegisterComponentLoadFunction("FleeController", LoadFleeController);
	loader.RegisterComponentLoadFunction("SkillSystemComponent", LoadSkillSystemComponent);
	loader.RegisterComponentLoadFunction("HealthComponent", LoadHealthComponent);
	loader.RegisterComponentLoadFunction("HealthBarComponent", LoadHealthBarComponent);
	loader.RegisterComponentLoadFunction("DropComponent", LoadDropComponent);
	loader.RegisterComponentLoadFunction("ManaComponent", LoadManaComponent);
	loader.RegisterComponentLoadFunction("SoundComponent", LoadSoundComponent);
	loader.RegisterComponentLoadFunction("InventoryComponent", LoadInventoryComponet);
	loader.RegisterComponentLoadFunction("MainStatComponent", LoadMainStatComponent);
	loader.RegisterComponentLoadFunction("PickupComponent", LoadPickerupperComponent);
	loader.RegisterComponentLoadFunction("EnemyObjective", LoadEnemyObjective);
	loader.RegisterComponentLoadFunction("ActivationObjectiveComponent", LoadActivationObjective);
	loader.RegisterComponentLoadFunction("ActivatorComponent", LoadActivationComponent);
	loader.RegisterComponentLoadFunction("ActivateShopComponent", LoadOpenShopListenerComponent);
	loader.RegisterComponentLoadFunction("ChangeLevelComponent", LoadChangeLevelTriggerComponent);
	loader.RegisterComponentLoadFunction("BlessingTowerComponent", LoadBlessingTowerComponent);
	loader.RegisterComponentLoadFunction("SpawnEnemiesOnDeathComponent", LoadSpawnEnemiesOnDeathComponent);
	loader.RegisterComponentLoadFunction("RespawnComponent", LoadRespawnComponent);
}

LoadManager::LoadManager()
{
	myCurrentScene = nullptr;
	myCurrentPlaystate = nullptr;

	RegisterFunctions();
}


LoadManager::~LoadManager()
{
	myCurrentScene = nullptr;
	myCurrentPlaystate = nullptr;
}
