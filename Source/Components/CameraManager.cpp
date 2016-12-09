#include "stdafx.h"
#include "CameraManager.h"
#include "ComponentManager.h"

CCameraManager* CCameraManager::myInstance = nullptr;

CCameraManager::CCameraManager()
{
	myCameraComponents.Init(1);
}


CCameraManager::~CCameraManager()
{
	for (unsigned int i = 0; i < myCameraComponents.Size(); i++)
	{
		CComponentManager::GetInstance().RemoveComponent(myCameraComponents[i]->GetId());
		SAFE_DELETE(myCameraComponents[i]);
	}

	myCameraComponents.RemoveAll();
}

void CCameraManager::Create()
{
	assert(myInstance == nullptr && "Camera manager already created");
	myInstance = new CCameraManager;
}

void CCameraManager::Destroy()
{
	assert(myInstance != nullptr && "Camera manager not created (is NULL)");
	SAFE_DELETE(myInstance);
}

CCameraManager& CCameraManager::GetInstance()
{
	assert(myInstance != nullptr && "Camera manager not created (is NULL)");
	return *myInstance;
}

CCameraComponent* CCameraManager::CreateCameraComponent()
{
	CCameraComponent* camera;
	camera = new CCameraComponent();

	CComponentManager::GetInstance().RegisterComponent(camera);

	if (myCameraComponents.Size() <= 0)
	{
		myActiveCamera = camera;
	}
	myCameraComponents.Add(camera);
	return  camera;
}

CU::Camera& CCameraManager::GetActiveCamera()
{
	myCamera.SetTransformation(myActiveCamera->GetToWorldTransformation());
	return myCamera;
}
