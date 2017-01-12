#include "stdafx.h"
#include "LoadMovementComponent.h"
#include "KevinLoader/KevinLoader.h"
#include "LoadManager.h"
#include "ComponentManager.h"
#include "MovementComponent.h"
#include "MovementComponentManager.h"

int LoadMovementComponent(KLoader::SLoadedComponentData someData)
{
	MovementComponent* Component = MovementComponentManager::GetInstance().CreateAndRegisterComponent();
	Component->SetMovementSpeed(someData.myData.at("MovementSpeed").GetFloat());
	return Component->GetId();
}
