#include "stdafx.h"
#include "LoadInventoryComponent.h"
#include "KevinLoader/KevinLoader.h"
#include "LoadManager.h"
#include "ComponentManager.h"
#include "InventoryComponent.h"
#include "InventoryComponentManager.h"

int LoadInventoryComponet(KLoader::SLoadedComponentData someData)
{
	CInventoryComponent* inventory = &CInventoryComponentManager::GetInstance().CreateAndRegisterComponent();

	

	return inventory->GetId();
}
