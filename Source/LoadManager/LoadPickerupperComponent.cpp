#include "stdafx.h"
#include "LoadPickerupperComponent.h"
#include "KevinLoader/KevinLoader.h"
#include "LoadManager.h"
#include "ComponentManager.h"
#include "../Game/PickerUpperComponent.h"
#include "../Game/PickupManager.h"

int LoadPickerupperComponent(KLoader::SLoadedComponentData someData)
{
	CPickerUpperComponent* pickup = CPickupManager::GetInstance().CreatePickerUpperComp();

	return pickup->GetId();
}
