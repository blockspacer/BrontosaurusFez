#include "stdafx.h"
#include "PollingStation.h"
#include "PlayerData.h"


CGameObject* PollingStation::playerObject = nullptr;
CComponent* PollingStation::PlayerInput = nullptr;
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
	playerData->myIsWhirlwinding = false;
	myThingsEnemiesShouldAvoid.RemoveAll();
}
