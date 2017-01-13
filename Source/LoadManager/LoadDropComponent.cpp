#include "stdafx.h"
#include "LoadDropComponent.h"
#include "KevinLoader/KevinLoader.h"
#include "LoadManager.h"
#include "ComponentManager.h"
#include "DropComponent.h"
#include "DropComponentManager.h"

int LoadDropComponent(KLoader::SLoadedComponentData someData)
{
	DropComponent* dropComp = DropComponentManager::GetInstance().CreateAndRegisterComponent(someData.myData.at("GoldToDropOnDeath").GetUInt());

	

	return dropComp->GetId();
}
