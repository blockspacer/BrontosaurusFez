#include "stdafx.h"
#include "Game.h" 

#include "PlayState.h"
#include "MainMenuState.h"
#include "SplashScreen.h" //rename to state?

#include "LoadManager/LuaFunctions.h"
#include "LuaWrapper/SSlua/SSlua.h"
#include "ScriptLoader.h"
#include "CommonUtilities/CommandLineManager.h"

#include "Engine.h"
#include "KevinLoader/KLoaderError.h"
#include "KevinLoader/KevinLoader.h"
#include "QuestManager.h"

#include "PollingStation.h"
#include "PlayerData.h"

CGame::CGame()
{
}


CGame::~CGame()
{
	KLoader::CKevinLoader::DestroyInstance();
	CBackgroundLoadingManager::DestroyInstance();
	SSlua::LuaWrapper::DestroyIfCreated();
}

void CGame::Init()
{
	 CBackgroundLoadingManager::CreateInstance();

	 KLoader::CKevinLoader::CreateInstance();

	 SSlua::LuaWrapper& luaWrapper = SSlua::LuaWrapper::GetInstance();
	 luaWrapper.RegisterFunctions(&ScriptLoader::RegisterLuaFunctions);
	 
	 PollingStation::playerData = new PlayerData;

	if (CommandLineManager::GetInstance()->HasParameter("-skipMainMenu") == true)
	{
		myStateStack.PushState(new CPlayState(myStateStack, 0));
	}
	else
	{
		myStateStack.PushState(new MainMenuState(myStateStack));
	}
	
	if (CommandLineManager::GetInstance()->HasParameter("-skipSplashScreen") == false)
	{
		mySplashScreen = new CSplashScreen(myStateStack);
	
		mySplashScreen->AddPicture("Sprites/SplashScreen/DOtga.dds");
		mySplashScreen->AddPicture("Sprites/SplashScreen/DOtimeDust.dds");
		mySplashScreen->AddPicture("Sprites/SplashScreen/doggo.dds");
	
	
		myStateStack.PushState(mySplashScreen);
	}
}

void CGame::Update(const CU::Time& aDeltaTime)
{
	bool isRunning = myStateStack.Update(aDeltaTime);
	if (isRunning == false)
	{
		CEngine::GetInstance()->Shutdown();
	}		
}

void CGame::Render()
{
	myStateStack.Render();
}