#pragma once
#include "../StateStack/State.h"
#include "../BrontosaurusEngine/StreakEmitterInstance.h"
//#include "../CommonUtilities/GrowingArray.h"
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

class ControllerComponent;
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

	bool GetIfLoadede() const;

private:
	void CreateManagersAndFactories();

	CScene myScene;
	
	CGameObjectManager* myGameObjectManager;
	GUI::GUIManager* myGUIManager;
	StatManager* myStatManager;
	CPointLightInstance* myPointLight; // Attached to camera for the moment
	//Lights::SDirectionalLight myDirectionalLight;
	//CU::GrowingArray<CPointLightInstance*> myPointLights;
	ControllerComponent*  myControllerComponent;
	CGameObject* myCameraObject;
	std::atomic<bool>  myIsLoaded;

	int myLevelIndex;
	bool myShouldReturnToLevelSelect;
};

inline bool CPlayState::GetIfLoadede() const
{
	return myIsLoaded;
}
