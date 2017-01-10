#include "stdafx.h"
#include "InputControllerManager.h"
#include "InputController.h"
#include "ComponentManager.h"
#include "../BrontosaurusEngine/Scene.h"

InputControllerManager* InputControllerManager::ourInstance = nullptr;

InputControllerManager::InputControllerManager()
{
	myComponents.Init(2);
}


InputControllerManager & InputControllerManager::GetInstance()
{
	assert(ourInstance != nullptr && "input controller component manager not created (is NULL)");

	return *ourInstance;
}

void InputControllerManager::CreateInstance()
{
	assert(ourInstance == nullptr && "input controller component manager already created");
	ourInstance = new InputControllerManager();
}

void InputControllerManager::DestroyInstance()
{

	assert(ourInstance != nullptr && "input controller component manager not created (is NULL)");

	SAFE_DELETE(ourInstance);
}

CU::GrowingArray<InputController*>& InputControllerManager::GetComponents()
{
	return myComponents;
}

void InputControllerManager::SetScene(CScene* aScene)
{
	myScene = aScene;
}

InputController* InputControllerManager::CreateAndRegisterComponent()
{
	InputController* Component;
	Component = new InputController(myScene->GetCamera(CScene::eCameraType::ePlayerOneCamera));
	myComponents.Add(Component);
	CComponentManager::GetInstance().RegisterComponent(Component);

	return Component;
}

void InputControllerManager::Update(const CU::Time & aDeltaTime)
{
	for (InputController* component : myComponents)
	{
		if (component != nullptr)
		{
			component->Update(aDeltaTime.GetSeconds());
		}
	}
}

InputControllerManager::~InputControllerManager()
{
	for (InputController* component : myComponents)
	{
		CComponentManager::GetInstance().RemoveComponent(component->GetId());
		SAFE_DELETE(component);
	}

	myComponents.RemoveAll();
}
