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
	assert(ourInstance != nullptr && "enemy ai controller component manager not created (is NULL)");

	return *ourInstance;
}

void InputControllerManager::CreateInstance()
{
	assert(ourInstance == nullptr && "enemy ai controller component manager already created");
	ourInstance = new InputControllerManager();
}

void InputControllerManager::DestroyInstance()
{

	assert(ourInstance != nullptr && "enemy ai controller component manager not created (is NULL)");

	SAFE_DELETE(ourInstance);
}

void InputControllerManager::RegisterComponent(InputController * aComponent)
{
	myComponents.Add(aComponent);
}

void InputControllerManager::Update(const CU::Time & aDeltaTime)
{
	for (unsigned short i = 0; i < myComponents.Size(); i++)
	{
		if (myComponents[i] != nullptr)
		{
			myComponents[i]->Update(aDeltaTime.GetSeconds());
		}
	}
}

InputControllerManager::~InputControllerManager()
{
	for (unsigned int i = 0; i < myComponents.Size(); ++i)
	{
		CComponentManager::GetInstance().RemoveComponent(myComponents[i]->GetId());
		SAFE_DELETE(myComponents[i]);
	}

	myComponents.RemoveAll();
}
