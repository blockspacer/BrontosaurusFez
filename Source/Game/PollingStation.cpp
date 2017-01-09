#include "stdafx.h"
#include "PollingStation.h"


CGameObject* PollingStation::playerObject = nullptr;
CComponent* PollingStation::PlayerInput = nullptr;
ValueObserver<int>* PollingStation::playerHealthBar = nullptr;
ValueObserver<int>* PollingStation::playerBoostBar = nullptr;
ValueObserver<int>* PollingStation::timeObjectiveBar = nullptr;
CollisionManager* PollingStation::collsionManager = nullptr;
CU::GrowingArray<WorldCenterComponent*> PollingStation::worldCenterComponents(4);

void PollingStation::NullifyLevelSpecificData()
{
	playerObject = nullptr;
	playerHealthBar = nullptr;
	playerBoostBar = nullptr;
	collsionManager = nullptr;

	if (worldCenterComponents.IsInitialized() == true)
	{
		//worldCenterComponents.DeleteAll();
		worldCenterComponents.Destroy();
	}
}
