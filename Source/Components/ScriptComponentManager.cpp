#include "stdafx.h"
#include "ScriptComponentManager.h"
#include "ScriptComponent.h"
#include "ComponentManager.h"

CScriptComponentManager* CScriptComponentManager::ourInstance = nullptr;

CScriptComponentManager::CScriptComponentManager()
	: myComponents(32)
	, myFunctionTableIndex(0)
{
	assert(ourInstance == nullptr && "There already exists a script component manager");
	ourInstance = this;
}

CScriptComponentManager::~CScriptComponentManager()
{
	assert(ourInstance == this && "There should not exists another script component manager");
	ourInstance = nullptr;

	myComponents.DeleteAll();
}

CScriptComponent* CScriptComponentManager::CreateComponent(const std::string& aScriptPath)
{
	CScriptComponent* newComponent = new CScriptComponent();
	ComponentId componentID = CComponentManager::GetInstance().RegisterComponent(newComponent);

	CScriptComponent::eInitSuccess error = newComponent->Init(aScriptPath, myFunctionTableIndex++);
	if (!CScriptComponent::HandleError(error))
	{
		delete newComponent;
		return nullptr;
	}



	return newComponent;
}

CScriptComponentManager* CScriptComponentManager::GetInstance()
{
	return ourInstance;
}
