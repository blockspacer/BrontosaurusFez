#include "stdafx.h"
#include "LoadOpenShopListenerComponent.h"
#include "KevinLoader/KevinLoader.h"
#include "LoadManager.h"
#include "ComponentManager.h"
#include "../Components/OpenShopListenerComponent.h"

int LoadOpenShopListenerComponent(KLoader::SLoadedComponentData someData)
{
	COpenShopListenerComponent* temp = new COpenShopListenerComponent();

	CComponentManager::GetInstance().RegisterComponent(temp);

	return temp->GetId();
}
