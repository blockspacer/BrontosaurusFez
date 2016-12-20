#include "stdafx.h"
#include "InputControllerManager.h"
#include "InputController.h"
#include "ComponentManager.h"

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

void InputControllerManager::RegisterComponent(InputController * aComponent)
{
	myComponents.Add(aComponent);
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
