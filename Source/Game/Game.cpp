#include "stdafx.h"
#include "Game.h" 

#include "PlayState.h"
#include "MainMenuState.h"
#include "SplashScreen.h" //rename to state?

#include "LoadManager/LuaFunctions.h"
#include "LuaWrapper/SSlua/SSlua.h"
#include "CommonUtilities/CommandLineManager.h"

#include "Engine.h"
#include "KevinLoader/KLoaderError.h"
#include "KevinLoader/KevinLoader.h"

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
	 //LuaFunctions::RegisterFunctions();
	 //SSlua::LuaWrapper::GetInstance().AddScriptPath("Scripts");
	 //SSlua::LuaWrapper::GetInstance().LoadCode("Scripts/GameMain.lua");
	 //SSlua::LuaWrapper::GetInstance().RunLoadedCode();

	 KLoader::CKevinLoader::CreateInstance();
	 
	 PollingStation::playerData = new PlayerData;
	 PollingStation::playerData->myGold = 0;

	if (CommandLineManager::GetInstance()->HasParameter("-skipMainMenu") == true)
	{
		myStateStack.PushState(new CPlayState(myStateStack, -1));
	}
	else
	{
		myStateStack.PushState(new MainMenuState(myStateStack));
	}
	
	//if (CommandLineManager::GetInstance()->HasParameter("-skipSplashScreen") == false)
	//{
	//	mySplashScreen = new CSplashScreen(myStateStack);
	//
	//	mySplashScreen->AddPicture("SplashScreen/DOtga.dds");
	//	mySplashScreen->AddPicture("SplashScreen/DOtimeDust.dds");
	//	mySplashScreen->AddPicture("SplashScreen/DOsplash.dds");
	//
	//
	//	myStateStack.PushState(mySplashScreen);
	//}
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