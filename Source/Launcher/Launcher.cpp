#include "stdafx.h"

#include <Game.h>
#include <Engine.h>
#include <CommandLineManager.h>
#include <DL_Debug.h>
#include <Time.h>
#include "../PostMaster/PostMaster.h"
#include "../BrontosaurusEngine/EDebugInfoDrawerFlags.h"
#include "../Audio/AudioInterface.h"

//#include <iostream>

void Init(int argc, char* argv[]);
unsigned int DebugDrawerFlags();


#ifndef _RETAIL_BUILD
//Comment out linebelow to deactivate console
#define USE_CONSOLE
#endif // !_RETAIL_BUILD

#ifdef USE_CONSOLE
	#pragma comment(linker, "/SUBSYSTEM:console")
#else
	#include <windows.h>
	#pragma comment(linker, "/SUBSYSTEM:windows")
#endif

#ifdef USE_CONSOLE
int main(int argc, char* argv[])
{
	Init(argc, argv);
	return 0;
}
#else
int WINAPI WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nShowCmd*/)
{
	Init(__argc, __argv);
	return 0;
}
#endif


void Init(int argc, char* argv[])
{
	CommandLineManager::GetInstance()->SetArguments(argc, argv);
	PostMaster::CreateInstance();

	{
		CGame game;
		SInitEngineParams engineParams;
		engineParams.myWindowParams.Width = 1920;
		engineParams.myWindowParams.Height = 1080;
		engineParams.myWindowParams.Name = L"Brontosaurus-BassKodd";
		engineParams.myWindowParams.Title = L"Brontosaurus-BassKodd";
		engineParams.myWindowParams.Fullscreen = false;

		engineParams.myThreadRender = true;

		int Iwidth;
		int Iheight;
		bool Bfullscreen;

		if (CommandLineManager::GetInstance()->HasParameter("-width") == true)
		{
			std::string width;
			width = CommandLineManager::GetInstance()->GetArgument("-width");
			Iwidth = std::stoi(width);
			engineParams.myWindowParams.Width = Iwidth;
		}
		if (CommandLineManager::GetInstance()->HasParameter("-height") == true)
		{
			std::string height;
			height = CommandLineManager::GetInstance()->GetArgument("-height");
			Iheight = std::stoi(height);
			engineParams.myWindowParams.Height = Iheight;
		}
		if (CommandLineManager::GetInstance()->HasParameter("-fullscreen") == true)
		{
			std::string fullscreen;
			fullscreen = CommandLineManager::GetInstance()->GetArgument("-fullscreen");
			if (fullscreen == "False")
			{
				Bfullscreen = false;
				//std::cout << "false" << std::endl;
			}
			else if (fullscreen == "True")
			{
				Bfullscreen = true;
				//std::cout << "True" << std::endl;
			}
			engineParams.myWindowParams.Fullscreen = Bfullscreen;
		}
		if (CommandLineManager::GetInstance()->HasParameter("-noThreadRender") == true)
		{
			std::string fullscreen;
			fullscreen = CommandLineManager::GetInstance()->GetArgument("-noThreadRender");

			engineParams.myThreadRender = false;
		}



		engineParams.myInitCallbackFunction = std::bind(&CGame::Init, &game);
		engineParams.myUpdateCallbackFunction = std::bind(&CGame::Update, &game, std::placeholders::_1);
		engineParams.myRenderCallbackFunction = std::bind(&CGame::Render, &game);
		engineParams.myDebugFlags = DebugDrawerFlags();

		CEngine::GetInstance()->Init(engineParams);
		CEngine::GetInstance()->Start();
	}

	CEngine::DestroyInstance();
	PostMaster::DestroyInstance();
	DL_Debug::Debug::Destroy();
	CommandLineManager::DestroyInstance();

	DumpMemoryLeeks();
}

unsigned int DebugDrawerFlags()
{
#ifdef _DEBUG
	return eDebugFlags_FPS | eDebugFlags_DrawCalls | eDebugFlags_MemoryUsage;
#else //NDEBUG || _RETAIL_BUILD
	return 0;
#endif // _DEBUG
}
