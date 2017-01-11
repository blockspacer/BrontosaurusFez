#include "stdafx.h"
#include "LoadInputController.h"
#include "KevinLoader/KevinLoader.h"
#include "LoadManager.h"
#include "ComponentManager.h"
#include "../Components/InputControllerManager.h"
#include "../Components/InputController.h"
#include "../Game/PollingStation.h"

int LoadInputController(KLoader::SLoadedComponentData someData)
{
	InputController* Controller = InputControllerManager::GetInstance().CreateAndRegisterComponent();
	PollingStation::PlayerInput = Controller;
	return Controller->GetId();
}
