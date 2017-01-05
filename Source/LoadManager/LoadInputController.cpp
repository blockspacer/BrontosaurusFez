#include "stdafx.h"
#include "LoadInputController.h"
#include "KevinLoader/KevinLoader.h"
#include "LoadManager.h"
#include "ComponentManager.h"
#include "../Components/InputControllerManager.h"
#include "../Components/InputController.h"

int LoadInputController(KLoader::SLoadedComponentData someData)
{
	InputController* Controller = InputControllerManager::GetInstance().CreateAndRegisterComponent();
	return Controller->GetId();
}
