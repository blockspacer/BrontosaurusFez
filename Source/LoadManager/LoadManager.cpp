#include "stdafx.h"
#include "LoadManager.h"
#include "DL_Debug.h"

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

LoadManager::LoadManager()
{
	myCurrentScene = nullptr;
	myCurrentPlaystate = nullptr;
}


LoadManager::~LoadManager()
{
	myCurrentScene = nullptr;
	myCurrentPlaystate = nullptr;
}
