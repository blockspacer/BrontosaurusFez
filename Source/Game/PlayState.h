#pragma once
#include "../StateStack/State.h"
#include "../PostMaster/Subscriber.h"

#include <atomic>

#include "../LuaWrapper/SSlua/SSlua.h"

//temp includes
#include <iostream>
#include "QuestDrawer.h"

namespace CU
{
	class Time;
}

namespace GUI
{
	class GUIManager;
}

class CScene;
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
class CParticleEmitterComponent;
class CCollisionComponentManager;

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

	CGameObjectManager* GetObjectManager() const;

	//TEMP - BELOW THIS LINE
	static SSlua::ArgumentList LuaFunction(SSlua::ArgumentList anArgumentList);

	void TEMP_ADD_HAT(CGameObject* aPlayerObject);
	CGameObject* myPlayerObject;

private:
	void CreateManagersAndFactories();

	CScene* myScene;
	
	CParticleEmitterComponent* myEmitterComp;

	CGameObjectManager* myGameObjectManager;
	CCollisionComponentManager* myCollisionComponentManager;
	GUI::GUIManager* myGUIManager;
	StatManager* myStatManager;
	
	//CGameObject* myCameraObject;

	int myLevelIndex;
	std::atomic_bool  myIsLoaded;
	bool myShouldReturnToLevelSelect;

	QM::CQuestDrawer myQuestDrawer;

	//TEMP - BELOW THIS LINE
	void TEMP_CREATE_ENEMY(); 
	CU::GrowingArray<CGameObject*> myEnemies;
};

inline bool CPlayState::IsLoaded() const
{
	return myIsLoaded;
}

inline SSlua::ArgumentList CPlayState::LuaFunction(SSlua::ArgumentList anArgumentList)
{
	std::cout << "Du anropade en function från Konsolen Punkt 3.a klar" << std::endl;
	return SSlua::ArgumentList();
}
