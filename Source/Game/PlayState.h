#pragma once
#include "../StateStack/State.h"
#include "../PostMaster/Subscriber.h"
#include "../BrontosaurusEngine/StreakEmitterInstance.h" //TODO: solve this including hell
#include "../BrontosaurusEngine/Lights.h"
#include "../BrontosaurusEngine/Scene.h"
#include "../Physics/PhysicsManager.h"

#include "ValueObserver.h"
#include <atomic>

namespace CU
{
	class Time;
}

namespace GUI
{
	class GUIManager;
}

class CGameObject;
class CGameObjectManager;
class WeaponSystemComponent;
class EnemyAIControllerComponent;
class CollisionManager;
class CPointLightInstance;
class CModelComponent;
class EnemyEmitterComponent;
class DeathComponentFactory;
class StatManager;
class CTextInstance;

class CPlayState : public State, public Subscriber
{
public:
	CPlayState(StateStack& aStateStack, const int aLevelIndex, const bool aShouldReturnToLevelSelect = false);
	~CPlayState();
	void Load();

	void Init() override;
	eStatus Update(const CU::Time& aDeltaTime) override;
	void Render() override;
	void OnEnter() override;
	void OnExit() override;
	void Pause();

	void NextLevel();

	eMessageReturn Recieve(const Message& aMessage) override;

	inline bool IsLoaded() const;

private:
	void CreateManagersAndFactories();

	CScene myScene;
	
	CGameObjectManager* myGameObjectManager;
	GUI::GUIManager* myGUIManager;
	StatManager* myStatManager;
	
	//CGameObject* myCameraObject;

	int myLevelIndex;
	std::atomic_bool  myIsLoaded;
	bool myShouldReturnToLevelSelect;
};

inline bool CPlayState::IsLoaded() const
{
	return myIsLoaded;
}
