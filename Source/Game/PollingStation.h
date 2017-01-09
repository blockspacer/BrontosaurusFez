#pragma once
class CGameObject;
class CollisionManager;

template <typename ValueType>
class ValueObserver;
struct PlayerData;
class CComponent;

struct PollingStation
{
	static void NullifyLevelSpecificData();
	static CComponent* PlayerInput;
	static CGameObject* playerObject;
	static PlayerData* playerData;
	static ValueObserver<int>* playerHealthBar;
	static ValueObserver<int>* playerBoostBar;
	static ValueObserver<int>* timeObjectiveBar;
	static CollisionManager* collsionManager;
};
