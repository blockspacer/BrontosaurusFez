#include "stdafx.h"
#include "RenderCamera.h"
#include "Renderer.h"
#include "Engine.h"
#include "RenderMessages.h"


CRenderCamera::CRenderCamera()
{
}


CRenderCamera::~CRenderCamera()
{
	
}

void CRenderCamera::Init(const float aFov, const float aWidth, const float aHeight, const float aFar, const float aNear, DXGI_FORMAT aFormat /*= DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM*/)
{
	myCamera.Init(aFov, aWidth, aHeight, aNear, aFar);
}

void CRenderCamera::Init(const float aWidth, const float aHeight, const float aFar, const float aNear, DXGI_FORMAT aFormat /*= DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM*/)
{
	myCamera.Init(aWidth, aHeight, aNear, aFar);

}

void CRenderCamera::Render()
{
	SSetCameraMessage* setCamMsg = new SSetCameraMessage();
	setCamMsg->myCamera = myCamera;
	RENDERER.AddRenderMessage(setCamMsg);
	SRenderCameraQueueMessage * camqueueMsg = new SRenderCameraQueueMessage();
	camqueueMsg->CameraRenderPackage = myRenderPackage;
	camqueueMsg->CameraRenderQueue = myRenderQueue;
	RENDERER.AddRenderMessage(camqueueMsg);

}

