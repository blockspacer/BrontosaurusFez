#include "stdafx.h"
#include "PollingStation.h"


CGameObject* PollingStation::playerObject = nullptr;
ValueObserver<int>* PollingStation::playerHealthBar = nullptr;
ValueObserver<int>* PollingStation::playerBoostBar = nullptr;
ValueObserver<int>* PollingStation::timeObjectiveBar = nullptr;
CollisionManager* PollingStation::collsionManager = nullptr;
PlayerData* PollingStation::playerData = nullptr;

void PollingStation::NullifyLevelSpecificData()
{
	playerObject = nullptr;
	playerHealthBar = nullptr;
	playerBoostBar = nullptr;
	collsionManager = nullptr;
}
