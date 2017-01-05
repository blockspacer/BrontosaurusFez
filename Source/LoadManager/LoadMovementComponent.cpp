#include "stdafx.h"
#include "LoadMovementComponent.h"
#include "KevinLoader/KevinLoader.h"
#include "LoadManager.h"
#include "ComponentManager.h"
#include "MovementComponent.h"
#include "MovementComponentManager.h"

int LoadMovementComponent(KLoader::SLoadedComponentData someData)
{
	CComponent* Component = MovementComponentManager::GetInstance().CreateAndRegisterComponent();
	return Component->GetId();
}
