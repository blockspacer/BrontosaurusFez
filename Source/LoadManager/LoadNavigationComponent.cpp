#include "stdafx.h"
#include "LoadNavigationComponent.h"
#include "KevinLoader/KevinLoader.h"
#include "LoadManager.h"
#include "ComponentManager.h"
#include "../Components/NavigationComponent.h"

int LoadNavigationComponent(KLoader::SLoadedComponentData someData)
{
	NavigationComponent* Navigation = new NavigationComponent();

	CComponentManager::GetInstance().RegisterComponent(Navigation);

	return Navigation->GetId();
}
