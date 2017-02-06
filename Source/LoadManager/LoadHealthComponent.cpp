#include "stdafx.h"
#include "LoadHealthComponent.h"
#include "KevinLoader/KevinLoader.h"
#include "LoadManager.h"
#include "ComponentManager.h"
#include "HealthComponent.h"
#include "JsonValue.h"

int LoadHealthComponent(KLoader::SLoadedComponentData someData)
{
	CHealthComponent* health = new CHealthComponent();
	CComponentManager::GetInstance().RegisterComponent(health);
	health->SetMaxHealth(someData.myData.at("MaxHealth").GetUInt());

	if (someData.myData.HasKey("ObjectType") == true)
	{
		health->SetObjectType(static_cast<eObjectType>(someData.myData.at("ObjectType").GetUInt()));
	}


	return health->GetId();
}
