#include "stdafx.h"
#include "Engine.h"
#include "WindowsWindow.h"
#include "DXFramework.h"
#include "InputManager.h"
#include "ModelManager.h"
#include "SpriteManager.h"
#include "ShaderManager.h"
#include "LightManager.h"
#include "TextureManager.h"
#include <Camera.h>
#include <CommonUtilities.h>
#include <TimerManager.h>
#include <ThreadPool.h>
#include <DL_Debug.h>
#include "Skybox.h"
#include "DDSTextureLoader.h"
#include "GUIRenderer.h"
#include "LineDrawer.h"
#include "Renderer.h"
#include "DebugInfoDrawer.h"
#include "ThreadNamer.h"
#include "../Audio/AudioInterface.h"
#include "../FontEngine/FontEngineFacade.h"

CEngine* CEngine::myInstance = nullptr;

CEngine* CEngine::GetInstance()
{
	if(myInstance == nullptr)
	{
		myInstance = new CEngine();
	}
	return myInstance;
}

void CEngine::DestroyInstance()
{
	SAFE_DELETE(myInstance);
}

void CEngine::Init(SInitEngineParams& aInitEngineParams)
{
	CU::TimerManager timerMgr;
	CU::TimerHandle handle = timerMgr.CreateTimer();
	timerMgr.StartTimer(handle);


	myTimerManager = new CU::TimerManager();
	myTimerH = myTimerManager->CreateTimer();
	myThreadPool = new CU::ThreadPool();
	
	myWindowsWindow = new CWindowsWindow(aInitEngineParams.myWindowParams);
	myWindowSize.x = aInitEngineParams.myWindowParams.Width;
	myWindowSize.y = aInitEngineParams.myWindowParams.Height;

	myThreadRender = aInitEngineParams.myThreadRender;

	myInitCallbackFunction = aInitEngineParams.myInitCallbackFunction;
	myUpdateCallbackFunction = aInitEngineParams.myUpdateCallbackFunction;
	myRenderCallbackFunction = aInitEngineParams.myRenderCallbackFunction;

	myDXFramework = new CDXFramework();
	myDXFramework->Initialize(aInitEngineParams.myWindowParams.Width, aInitEngineParams.myWindowParams.Height, aInitEngineParams.myWindowParams.Fullscreen ,myWindowsWindow->GetHWND());
	myInputManager = new CInputManager();
	myModelManager = new CModelManager();
	mySpriteManager = new CSpriteManager();
	myGUIRenderer = new CGUIRenderer();
	myShaderManager = new CShaderManager();
	myLightManager = new CLightManager();
	myTextureManager = new CTextureManager();

	myCamera = new CU::Camera(60 , aInitEngineParams.myWindowParams.Width, aInitEngineParams.myWindowParams.Height, 1.f, 75000.0f,{ 0.0f, 0.0f, 0.f });
	myRenderer = new CRenderer();

	myLineDrawer = new CLineDrawer();
	CFontEngineFacade::CreateInstance();
	myFontEngine = CFontEngineFacade::GetInstance();
	myDebugInfoDrawer = new CDebugInfoDrawer(aInitEngineParams.myDebugFlags);
	

	ShowCursor(TRUE);

	bool result;
	Audio::CAudioInterface::CreateInstance();
	result = Audio::CAudioInterface::GetInstance()->Init("Audio/Init.bnk");

	timerMgr.UpdateTimers();
	float time = timerMgr.GetTimer(handle).GetLifeTime().GetMilliseconds();
	ENGINE_LOG("Engine Inited in %f ms", time);
}

void CEngine::Render()
{
	myDXFramework->ClearDepthStencil();
	myDXFramework->ClearScreen();

	//myDebugInfoDrawer->Update();
	//myDebugInfoDrawer->Render(myWindowSize);

	myRenderer->Render();

	myLineDrawer->Render();
	myDXFramework->Render();
}

void CEngine::ThreadedRender()
{
	CU::SetThreadName("Render thread");
	while (CEngine::GetInstance()->GetIsRunning())
	{
		Render();
	}
}

void CEngine::OnResize(const unsigned int aWidth, const unsigned int aHeight)
{
	myWindowSize.x = aWidth;
	myWindowSize.y = aHeight;

	delete myRenderer;
	myDXFramework->Resize(aWidth, aHeight);
	myDXFramework->SetViewPort(aWidth, aHeight, 0.f, 1.f, 0.f, 0.f);

	myCamera->ReInit(60, static_cast<float>(aWidth), static_cast<float>(aHeight), 1.f, 75000.0f); //TODO: remove near and far, use the same derp (maybe add new func specifically for that)
	myRenderer = new CRenderer();
}

void CEngine::Start()
{
	myInitCallbackFunction();

	if (myThreadRender == true)
	{
		auto renderThread = [this]()
		{
			ThreadedRender();
		};
		myThreadPool->AddWork(CU::Work(renderThread, CU::ePriority::eHigh));
	}

	while (CEngine::GetInstance()->GetIsRunning())
	{
		myTimerManager->UpdateTimers();
		myInputManager->Update();

		myRenderer->SwapWrite();


		myUpdateCallbackFunction(myTimerManager->GetTimer(myTimerH).GetDeltaTime());
		myWindowsWindow->Update();
		


		Audio::CAudioInterface* audio = Audio::CAudioInterface::GetInstance();
		if (audio != nullptr)
		{
			audio->SetListenerPosition(myCamera->GetTransformation());
			audio->Update();
		}

	
		myRenderCallbackFunction();

		if (myThreadRender == false)
		{
			Render();
		}

	}
}

bool CEngine::GetIsRunning()
{
	return myWindowsWindow->GetIsWindowOpen();
}

void CEngine::ClearBackbuffer()
{
	myDXFramework->ClearScreen();
}




void CEngine::Shutdown()
{
	myWindowsWindow->Close();
}

CEngine::CEngine()
	: myGUIRenderer(nullptr)
	, myLineDrawer(nullptr)
	, myDebugInfoDrawer(nullptr)
{
	myGameRef = nullptr;
}

CEngine::~CEngine()
{
	SAFE_DELETE(myRenderer);
	SAFE_DELETE(myModelManager);
	SAFE_DELETE(mySpriteManager);
	SAFE_DELETE(myCamera);
	SAFE_DELETE(myDXFramework);
	SAFE_DELETE(myGUIRenderer);
	SAFE_DELETE(myDebugInfoDrawer);
	SAFE_DELETE(myTimerManager);
	SAFE_DELETE(myWindowsWindow);
	SAFE_DELETE(myInputManager);
	SAFE_DELETE(myShaderManager);
	SAFE_DELETE(myLightManager);
	SAFE_DELETE(myTextureManager);
	SAFE_DELETE(myLineDrawer);
	SAFE_DELETE(myThreadPool); //TODO: THREAD POOL HAS THREADS IT CANNOT JOIN

	Audio::CAudioInterface::Destroy();
	// TODO: Solve this hell
	//myCubeMap->Release();
}


CU::Time CEngine::GetTime()
{
	return myTimerManager->GetTimer(myTimerH).GetLifeTime();
}
