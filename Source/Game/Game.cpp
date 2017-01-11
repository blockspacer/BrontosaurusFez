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
#include "QuestManager.h"


CGame::CGame(): mySplashScreen(nullptr)
{
}


CGame::~CGame()
{
	KLoader::CKevinLoader::DestroyInstance();
	CBackgroundLoadingManager::DestroyInstance();
	SSlua::LuaWrapper::DestroyIfCreated();
	QM::CQuestManager::DestroyInstance();

}

void CGame::Init()
{
	 CBackgroundLoadingManager::CreateInstance();
	 QM::CQuestManager::CreateInstance();

	 KLoader::CKevinLoader::CreateInstance();

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