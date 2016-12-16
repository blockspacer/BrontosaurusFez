#pragma once
//#include "stdafx.h"
#include <functional>
#include "../CommonUtilities/vector2.h"

class CParticleEmitterManager;
class CModelManager;
class CSpriteManager;
class CLineDrawer;
class CWindowsWindow;
class CDXFramework;
class CShaderManager;
class CLightManager;
class CInputManager;
class CTextureManager;
class CRenderer;
class CDebugInfoDrawer;


#define ENGINE 			CEngine::GetInstance()
#define WINDOW 			CEngine::GetInstance()->GetWindow()
#define HWNDGET			CEngine::GetInstance()->GetWindow()->GetHWND()
#define HINSTGET		CEngine::GetInstance()->GetWindow()->GetHinstance()

#define CAMERA 			CEngine::GetInstance()->GetCamera()
#define RENDERER		CEngine::GetInstance()->GetRenderer()
#define THREADPOOL		CEngine::GetInstance()->GetThreadPool()
#define LIGHTMANAGER 	CEngine::GetInstance()->GetLightManager()
#define TIMERMGR 		CEngine::GetInstance()->GetTimerManager()
#define MODELMGR 		CEngine::GetInstance()->GetModelManager()
#define SPRMGR			CEngine::GetInstance()->GetSpriteManager()
#define SHADERMGR 		CEngine::GetInstance()->GetShaderManager()
#define TEXTUREMGR 		CEngine::GetInstance()->GetTextureManager()

#define FRAMEWORK 		CEngine::GetInstance()->GetFramework()
#define DEVICE			CEngine::GetInstance()->GetFramework()->GetDevice()
#define DEVICE_CONTEXT	CEngine::GetInstance()->GetFramework()->GetDeviceContext()

#define WINDOW_SIZE     CEngine::GetInstance()->GetWindowSize()

namespace CU
{
	class TimerManager;
	class ThreadPool;
	class Camera;
	class Time;
	typedef unsigned int TimerHandle;
}


struct SInitWindowParams
{
	std::wstring Name;
	std::wstring Title;
	unsigned short Width;
	unsigned short Height;
	bool Fullscreen;
};

struct SInitEngineParams
{
	SInitWindowParams myWindowParams;
	std::function<void()> myInitCallbackFunction;
	std::function<void(const CU::Time&)> myUpdateCallbackFunction;
	std::function<void()>  myRenderCallbackFunction;
	unsigned int myDebugFlags;
	bool myThreadRender;
};

class CEngine
{
public:
	static CEngine* GetInstance();
	static void DestroyInstance();
	void Init(SInitEngineParams& aInitEngineParams);
	void Start();
	void Shutdown();

	bool GetIsRunning();
	void ClearBackbuffer();

	inline CRenderer& GetRenderer();
	inline CTextureManager& GetTextureManager();
	inline CLineDrawer& GetLineDrawer();
	inline CShaderManager * GetShaderManager();
	inline CModelManager* GetModelManager();
	inline CSpriteManager& GetSpriteManager();
	inline CWindowsWindow* GetWindow();
	inline CDXFramework* GetFramework();
	inline CLightManager * GetLightManager();
	inline CU::Camera* GetCamera();
	inline CU::TimerManager* GetTimerManager();
	inline CU::ThreadPool* GetThreadPool();
	inline CParticleEmitterManager& GetParticleEmitterManager();

	CU::Time GetTime();

	void Render();
	void ThreadedRender();
	void OnResize(const unsigned int aWidth, const unsigned int aHeight);
	inline const CU::Vector2ui& GetWindowSize();


	inline void SetCamera(CU::Camera* aCamera);

private:
	CEngine();
	~CEngine();


private:
	CWindowsWindow* myWindowsWindow;
	CModelManager* myModelManager;
	CSpriteManager* mySpriteManager;
	CLineDrawer* myLineDrawer;
	CShaderManager* myShaderManager;
	CLightManager* myLightManager;
	CInputManager* myInputManager;
	CTextureManager* myTextureManager;
	CParticleEmitterManager* myParticleEmitterManager;
	
	CDebugInfoDrawer* myDebugInfoDrawer;
	
	CU::TimerManager* myTimerManager;
	CU::TimerHandle myTimerH;

	std::function<void()>  myInitCallbackFunction;
	std::function<void(const CU::Time&)>  myUpdateCallbackFunction;
	std::function<void()>  myRenderCallbackFunction;
	
	CU::Camera* myCamera;
	CU::Vector2ui myWindowSize;

	CDXFramework* myDXFramework;
	CU::ThreadPool* myThreadPool;

	static CEngine* myInstance;

	CRenderer* myRenderer;

	bool myThreadRender;

};

CWindowsWindow* CEngine::GetWindow()
{
	return myWindowsWindow;
}

CDXFramework* CEngine::GetFramework()
{
	return myDXFramework;
}

inline CU::Camera* CEngine::GetCamera()
{
	return myCamera;
}

inline CModelManager* CEngine::GetModelManager()
{
	return myModelManager;
}

inline CSpriteManager& CEngine::GetSpriteManager()
{
	assert(mySpriteManager != nullptr && "Sprite manager not created (is NULL)");
	return *mySpriteManager;
}

inline CTextureManager & CEngine::GetTextureManager()
{
	return *myTextureManager;
}

inline CLineDrawer& CEngine::GetLineDrawer()
{
	assert(myLineDrawer != nullptr);
	return *myLineDrawer;
}

inline CShaderManager* CEngine::GetShaderManager()
{
	return myShaderManager;
}

inline CU::TimerManager* CEngine::GetTimerManager()
{
	return myTimerManager;
}

inline CU::ThreadPool* CEngine::GetThreadPool()
{
	return myThreadPool;
}

inline CParticleEmitterManager & CEngine::GetParticleEmitterManager()
{
	assert(myParticleEmitterManager != nullptr);
	return *myParticleEmitterManager;
}

inline CLightManager*CEngine::GetLightManager()
{
	return myLightManager;
}

inline void CEngine::SetCamera(CU::Camera* aCamera)
{
	myCamera = aCamera;
}

inline const CU::Vector2ui& CEngine::GetWindowSize()
{
	return myWindowSize;
}

inline CRenderer & CEngine::GetRenderer()
{
	return *myRenderer;
}
