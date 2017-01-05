#pragma once
#include <Camera.h>
#include "RenderPackage.h"

struct  SRenderMessage;

class CRenderCamera
{
public:
	CRenderCamera();
	~CRenderCamera();


	// WHAT IS NEEDED FOR INIT OF CAMERA?

	void Init(const float aFov, const float aWidth, const float aHeight, const float aFar, const float aNear, DXGI_FORMAT aFormat = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
	void Init(const float aWidth, const float aHeight, const float aFar, const float aNear, DXGI_FORMAT aFormat = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);


	inline void SetCamera(const CU::Camera& aCamera);
	inline const CU::Camera& GetCamera() const ;

	inline const CRenderPackage& GetRenderPackage() const;

	inline void AddRenderMessage(SRenderMessage* aRenderMessage);
	void Render();

private:
	CU::Camera myCamera;
	CRenderPackage myRenderPackage;
	CU::GrowingArray<SRenderMessage*, unsigned short, false> myRenderQueue;
};

inline const CRenderPackage & CRenderCamera::GetRenderPackage() const
{
	return myRenderPackage;
}

inline void CRenderCamera::AddRenderMessage(SRenderMessage * aRenderMessage)
{
	myRenderQueue.Add(aRenderMessage);
}

inline void CRenderCamera::SetCamera(const CU::Camera & aCamera)
{
	myCamera = aCamera;
}

inline const CU::Camera & CRenderCamera::GetCamera() const
{
	return myCamera;
}
