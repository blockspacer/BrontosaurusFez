#include "stdafx.h"
#include "PointLightComponentManager.h"
#include "PointLightComponent.h"
#include "ComponentManager.h"
#include "Scene.h"

CPointLightComponentManager* CPointLightComponentManager::ourInstance = nullptr;

void CPointLightComponentManager::Destroy()
{
	SAFE_DELETE(ourInstance);
}

void CPointLightComponentManager::Create(CScene& aScene)
{
	if (ourInstance == nullptr)
	{
		ourInstance = new CPointLightComponentManager(aScene);
	}
}

CPointLightComponentManager & CPointLightComponentManager::GetInstance()
{
	return *ourInstance;
}

PointLightComponent * CPointLightComponentManager::CreateAndRegisterComponent()
{
	PointLightComponent* pointLight = new PointLightComponent;
	CComponentManager::GetInstance().RegisterComponent(pointLight);
	myComponents.Add(pointLight);
	myScene;//add light to scene;
	return pointLight;
}

CPointLightComponentManager::CPointLightComponentManager(CScene& aScene)
{
	myComponents.Init(10);
	myScene = &aScene;
}


CPointLightComponentManager::~CPointLightComponentManager()
{
}
