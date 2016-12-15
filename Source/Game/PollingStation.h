#pragma once
class CGameObject;
class CollisionManager;

template <typename ValueType>
class ValueObserver;
class WorldCenterComponent;

struct PollingStation
{
	static void NullifyLevelSpecificData();

	static CGameObject* playerObject;
	static ValueObserver<int>* playerHealthBar;
	static ValueObserver<int>* playerBoostBar;
	static ValueObserver<int>* timeObjectiveBar;
	static CollisionManager* collsionManager;
	static CU::GrowingArray<WorldCenterComponent*> worldCenterComponents;
};