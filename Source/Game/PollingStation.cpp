#include "stdafx.h"
#include "PollingStation.h"


CGameObject* PollingStation::playerObject = nullptr;
CComponent* PollingStation::PlayerInput = nullptr;
ValueObserver<int>* PollingStation::playerHealthBar = nullptr;
ValueObserver<int>* PollingStation::playerBoostBar = nullptr;
ValueObserver<int>* PollingStation::timeObjectiveBar = nullptr;
CollisionManager* PollingStation::collsionManager = nullptr;
CNavmesh* PollingStation::Navmesh = nullptr;
PlayerData* PollingStation::playerData = nullptr;
CU::GrowingArray<CGameObject*> PollingStation::myThingsEnemiesShouldAvoid(100);


void PollingStation::NullifyLevelSpecificData()
{
	playerObject = nullptr;
	playerHealthBar = nullptr;
	playerBoostBar = nullptr;
	collsionManager = nullptr;
	myThingsEnemiesShouldAvoid.RemoveAll();
}
