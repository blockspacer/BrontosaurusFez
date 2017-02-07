#pragma once
#include "../StateStack/State.h"
#include "../PostMaster/Subscriber.h"

#include <atomic>

//#include "../LuaWrapper/SSlua/SSlua.h"

//temp includes
#include "QuestDrawer.h"
#include "../PostMaster/Event.h"
#include "QuestManager.h"

#include "../BrontosaurusEngine/Navmesh.h"

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

class CCollisionComponentManager;
class CScene;
class CHatmaker;
class CGameObject;
//class StatManager;
class CShopStorage;
class CTextInstance;
class CTextInstance;
class CModelComponent;
class CollisionManager;
class CGameObjectManager;
class CPointLightInstance;
class EnemyEmitterComponent;
class WeaponSystemComponent;
class DeathComponentFactory;
class CParticleEmitterComponent;
class EnemyAIControllerComponent;
class CHealthBarComponentManager;
class CScriptComponentManager;
class CParticleEffectManager;

class CPlayState : public State, public Subscriber
{
public:
	CPlayState(StateStack& aStateStack, const int aLevelIndex, const bool aShouldReturnToLevelSelect = false);
	~CPlayState();
	void Load();

	void Init() override;
	eStateStatus Update(const CU::Time& aDeltaTime) override;
	void Render() override;
	void OnEnter() override;
	void OnExit(const bool aLetThroughRender) override;
	void Pause();
	void BuyHats();
	void ChangeGoldAmount(const int aValue, const bool aDecreaseGold);

	void CheckReturnToLevelSelect();

	eMessageReturn Recieve(const Message& aMessage) override;



	inline bool IsLoaded() const;

	CGameObjectManager* GetObjectManager() const;
	CHealthBarComponentManager* GetHealthBarManager();
	CCollisionComponentManager* GetCollisionManager();

	//TEMP - BELOW THIS LINE

	QM::EventHandle fristObjective;
	QM::EventHandle secondObjective;
	QM::EventHandle thridObjective;
	bool myShuldRenderNavmesh;

private:
	void CreateManagersAndFactories();

	CScene* myScene;
	
	CParticleEmitterComponent* myEmitterComp;

	CGameObjectManager* myGameObjectManager;
	CCollisionComponentManager* myCollisionComponentManager;
	CScriptComponentManager* myScriptComponentManager;
	CParticleEffectManager* myParticleEffectManager;
	GUI::GUIManager* myGUIManager;
	//StatManager* myStatManager;
	CTextInstance* myGoldText;
	CU::GrowingArray<CTextInstance*> myChangeTexts;
	CHatMaker* myHatMaker;
	CShopStorage* myShopStorage;
	//CGameObject* myCameraObject;
	CMouseComponent* myMouseComponent;

	int myLevelIndex;
	std::atomic_bool  myIsLoaded;
	bool myShouldReturnToLevelSelect;

	QM::CQuestManager myQuestManager;
	QM::CQuestDrawer myQuestDrawer;
	//TEMP - BELOW THIS LINE
	CNavmesh myNavmesh;
	CU::GrowingArray<CGameObject*> myEnemies;
	CHealthBarComponentManager* myHealthBarManager;
};

inline bool CPlayState::IsLoaded() const
{
	return myIsLoaded;
}

