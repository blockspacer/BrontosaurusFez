#include "stdafx.h"
#include "PollingStation.h"
#include "PlayerData.h"


CGameObject* PollingStation::playerObject = nullptr;
CComponent* PollingStation::PlayerInput = nullptr;
ValueObserver<int>* PollingStation::playerHealthBar = nullptr;
ValueObserver<int>* PollingStation::playerBoostBar = nullptr;
ValueObserver<int>* PollingStation::timeObjectiveBar = nullptr;
CollisionManager* PollingStation::collsionManager = nullptr;
CNavmesh* PollingStation::Navmesh = nullptr;
PlayerData* PollingStation::playerData = nullptr;
CU::GrowingArray<CGameObject*> PollingStation::myThingsEnemiesShouldAvoid(100);
CU::GrowingArray<std::string> PollingStation::playerHatList(12);
std::string PollingStation::currentDialog = "";
CU::GrowingArray<int, char> PollingStation::OpenPortals(1);
std::map<std::string, unsigned int> PollingStation::QuestProgress;

void PollingStation::NullifyLevelSpecificData()
{
	playerObject = nullptr;
	playerHealthBar = nullptr;
	playerBoostBar = nullptr;
	collsionManager = nullptr;
	playerData->myIsWhirlwinding = false;
	myThingsEnemiesShouldAvoid.RemoveAll();
}
