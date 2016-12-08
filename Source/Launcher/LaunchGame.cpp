#include "stdafx.h"
#include "LaunchGame.h"

#include <Game.h>
#include <Engine.h>
#include <CommandLineManager.h>
#include <Time.h>
#include "../PostMaster/PostMaster.h"
#include "../BrontosaurusEngine/EDebugInfoDrawerFlags.h"
#include "../Audio/AudioInterface.h"

LaunchGame::LaunchGame()
{

}

LaunchGame::~LaunchGame()
{
}

void LaunchGame::Start(const int aWindowHeight, const int aWindowWidth, const bool aIsFullScreen)
{

	PostMaster::CreateInstance();
	{
		CGame game;
		SInitEngineParams engineParams;
		engineParams.myWindowParams.Width = aWindowWidth;
		engineParams.myWindowParams.Height = aWindowHeight;
		engineParams.myWindowParams.Name = L"Brontosaurus-BassKodd";
		engineParams.myWindowParams.Title = L"Brontosaurus-BassKodd";
		engineParams.myWindowParams.Fullscreen = aIsFullScreen;

		engineParams.myInitCallbackFunction = std::bind(&CGame::Init, &game);
		engineParams.myUpdateCallbackFunction = std::bind(&CGame::Update, &game, std::placeholders::_1);
		engineParams.myRenderCallbackFunction = std::bind(&CGame::Render, &game);
		engineParams.myDebugFlags = 0;

		CEngine::GetInstance()->Init(engineParams);
		CEngine::GetInstance()->Start();
	}

	CEngine::DestroyInstance();
	PostMaster::DestroyInstance();
	DL_Debug::Debug::Destroy();
	CommandLineManager::DestroyInstance();

	DumpMemoryLeeks();
}
