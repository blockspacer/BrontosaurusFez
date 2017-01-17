#pragma once
#include "../StateStack/State.h"
#include "../PostMaster/Subscriber.h"

#include <atomic>

//#include "../LuaWrapper/SSlua/SSlua.h"

//temp includes
#include "QuestDrawer.h"
#include "../PostMaster/Event.h"
#include "QuestManager.h"

namespace CU
{
	class Time;
}

namespace GUI
{
	class GUIManager;
}

namespace QM
{
	typedef int EventHandle;
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
class CTextInstance;
class CHealthBarComponentManager;
class CHatmaker;

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
	void BuyHats();
	void GiveHatToPlayer();

	void NextLevel();

	eMessageReturn Recieve(const Message& aMessage) override;



	inline bool IsLoaded() const;

	CGameObjectManager* GetObjectManager() const;
	CHealthBarComponentManager* GetHealthBarManager();
	CCollisionComponentManager* GetCollisionManager();

	//TEMP - BELOW THIS LINE
	void TEMP_ADD_HAT(CGameObject* aPlayerObject);
	CGameObject* myPlayerObject;

	QM::EventHandle fristObjective;
	QM::EventHandle secondObjective;
	QM::EventHandle thridObjective;

private:
	void CreateManagersAndFactories();

	CScene* myScene;
	
	CParticleEmitterComponent* myEmitterComp;

	CGameObjectManager* myGameObjectManager;
	CCollisionComponentManager* myCollisionComponentManager;
	GUI::GUIManager* myGUIManager;
	StatManager* myStatManager;
	CTextInstance* myGoldText;
	CHatMaker* myHatMaker;

	//CGameObject* myCameraObject;
	CMouseComponent* myMouseComponent;

	int myLevelIndex;
	std::atomic_bool  myIsLoaded;
	bool myShouldReturnToLevelSelect;

	QM::CQuestManager myQuestManager;
	QM::CQuestDrawer myQuestDrawer;

	//TEMP - BELOW THIS LINE
	void TEMP_CREATE_ENEMY(); 
	CU::GrowingArray<CGameObject*> myEnemies;
	CHealthBarComponentManager* myHealthBarManager;
};

inline bool CPlayState::IsLoaded() const
{
	return myIsLoaded;
}

