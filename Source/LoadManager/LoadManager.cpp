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
	loader.RegisterComponentLoadFunction("fleeController", LoadFleeController);
	loader.RegisterComponentLoadFunction("SkillSystemComponent", LoadSkillSystemComponent);
	loader.RegisterComponentLoadFunction("HealthComponent", LoadHealthComponent);
	loader.RegisterComponentLoadFunction("HealthBarComponent", LoadHealthBarComponent);

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
