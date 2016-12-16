#pragma once
#include "CameraComponent.h"
#include "Component.h"
#include "../CommonUtilities/Camera.h"

class CCameraManager
{
public:
	~CCameraManager();
	static void Create();
	static void Destroy();
	static CCameraManager& GetInstance();
	CCameraComponent* CreateCameraComponent();
	CU::Camera& GetActiveCamera();

private: 
	CCameraManager();
	static CCameraManager* myInstance;
	CU::Camera myCamera;
	CU::GrowingArray<CCameraComponent*> myCameraComponents;
	CCameraComponent* myActiveCamera;
};

