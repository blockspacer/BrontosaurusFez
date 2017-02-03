#include "stdafx.h"
#include "LoadHealthWellComponent.h"
#include "KevinLoader/KevinLoader.h"
#include "LoadManager.h"
#include "ComponentManager.h"
#include "HealthRestoreTriggerComponent.h"
#include "HealthRestoreTriggerComponentManager.h"

int LoadHealthWellComponent(KLoader::SLoadedComponentData someData)
{
	HealthRestoreTriggerComponent* healing = CHealthRestoreTriggerComponentManager::GetInstance().CreateAndRegisterComponent();
	healing->SetCoolDown(someData.myData.at("CoolDown").GetFloat());
	healing->SetHealAmount(someData.myData.at("HealAmount").GetUInt());

	return healing->GetId();
}
