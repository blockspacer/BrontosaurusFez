#include "stdafx.h"
#include "LoadActivationComponent.h"
#include "KevinLoader/KevinLoader.h"
#include "ActivationComponent.h"
#include "ComponentManager.h"

int LoadActivationComponent(KLoader::SLoadedComponentData)
{
	CActivationComponent* activationComponent = new CActivationComponent();
	return CComponentManager::GetInstance().RegisterComponent(activationComponent);
}
