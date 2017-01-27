#include "stdafx.h"
#include "PlayState.h"

#include <Engine.h> //fixa camera instance sen
#include <CommonUtilities.h>
#include <Lights.h>
#include <TimerManager.h>
#include <Renderer.h>
#include "Skybox.h"
#include <Scene.h>

//Managers
#include "QuestManager.h"
#include "CameraManager.h"
#include "InputControllerManager.h"
#include "MovementComponentManager.h"
#include "FleeControllerManager.h"
#include "SeekControllerManager.h"
#include "PickupFactory.h"
#include "PickupManager.h"
#include "PickerUpperComponent.h"
#include "EnemyFactory.h"
#include "AIControllerManager.h"

#include "Components/GameObjectManager.h"
#include "Components/AudioSourceComponentManager.h"
#include "Components/ModelComponentManager.h"
#include "Components/GameObject.h"
#include "Components/ModelComponent.h"
#include "Components/ParticleEmitterComponentManager.h"
#include "Components/ComponentManager.h"
#include "Components/PlayerHealthMessenger.h"
#include "Components/RespawnComponent.h"
#include "Components/RespawnComponentManager.h"
#include "Components/SkillFactory.h"
#include "Components/SkillSystemComponentManager.h"
#include "Components/CollisionComponentManager.h"

#include "../GUI/GUIManager.h"

#include "LoadManager/LoadManager.h"
//--------
#include "PostMaster/PopCurrentState.h"
#include "PostMaster/ChangeLevel.h"
#include "PostMaster/PostMaster.h"
#include "PostMaster/PushState.h"
#include "PostMaster/Message.h"
#include "PostMaster/Event.h"

#include "PollingStation.h"

#include "BrontosaurusEngine/LineDrawer.h"
#include "BrontosaurusEngine/TextInstance.h"

#include "../LuaWrapper/SSlua/SSlua.h"



#include "../Audio/AudioInterface.h"

#include "ShopStorage.h"

#include "PlayerData.h"

#include "MasterAI.h"

#include <time.h>
//Kanske Inte ska vara här?
#include "../BrontosaurusEngine/Console.h"
#include "ManaComponentManager.h"
//

//Temp Includes
#include "HatMaker.h"
#include "KevinLoader/KevinLoader.h"
#include "SkillComponentManager.h"
#include "DropComponentManager.h"
#include "../Collision/Intersection.h"
#include "Components/CollisionComponent.h"
#include "Collision/ICollider.h"
#include "SkillData.h"

#include "Components/HealthBarComponentManager.h"
#include "ManaComponent.h"
#include "LevelManager.h"
#include "MouseComponent.h"
#include "PlayerHealthMessenger.h"
#include "PlayerManaMessenger.h"
#include "ComponentMessage.h"

//ULTRA TEMP INCLUDES, remove if you see and remove the things that don't compile afterwards

CPlayState::CPlayState(StateStack& aStateStack, const int aLevelIndex, const bool aShouldReturnToLevelSelect)
	: State(aStateStack)
	, myLevelIndex(aLevelIndex)
	, myShouldReturnToLevelSelect(aShouldReturnToLevelSelect)
	, myScene(nullptr)
	, myMouseComponent(nullptr)
	, myQuestManager()
	, myQuestDrawer(myQuestManager)
	, myShuldRenderNavmesh(false)
{
	myIsLoaded = false;
	PostMaster::GetInstance().Subscribe(this, eMessageType::eHatAdded);
	PostMaster::GetInstance().Subscribe(this, eMessageType::eKeyboardMessage);
}

CPlayState::~CPlayState()
{
	//Don forgetti to deletti
	SAFE_DELETE(myEmitterComp);
	SAFE_DELETE(myCollisionComponentManager);
	SAFE_DELETE(myStatManager);
	SAFE_DELETE(myGoldText);
	SAFE_DELETE(myHatMaker);
	SAFE_DELETE(myHealthBarManager);
	SAFE_DELETE(myHatMaker);


	SAFE_DELETE(myMouseComponent);
	SAFE_DELETE(myScene);
	SAFE_DELETE(myGameObjectManager);
	SAFE_DELETE(myGUIManager);
	
	CModelComponentManager::Destroy();
	CAudioSourceComponentManager::Destroy();
	CParticleEmitterComponentManager::Destroy();
	CCameraComponentManager::Destroy();
	InputControllerManager::DestroyInstance();
	MovementComponentManager::DestroyInstance();
	SkillSystemComponentManager::DestroyInstance();
	AIControllerManager::Destroy();
	FleeControllerManager::Destroy();
	CSeekControllerManager::Destroy();
	SkillComponentManager::DestroyInstance();
	DropComponentManager::DestroyInstance();
	PollingStation::NullifyLevelSpecificData();
	ManaComponentManager::DestroyInstance();
	CShopStorage::Destroy();
	CPickupFactory::Destroy();
	CPickupManager::DestroyInstance();
	RespawnComponentManager::Destroy();
	CMasterAI::Destroy();

	SkillFactory::DestroyInstance();
	CComponentManager::DestroyInstance();
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eHatAdded);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eKeyboardMessage);

	CLevelManager::DestroyInstance();
}

void CPlayState::Load()
{
	//start taking the time for loading level
	CU::TimerManager timerMgr;
	CU::TimerHandle handle = timerMgr.CreateTimer();
	timerMgr.StartTimer(handle);
	srand(time(NULL));

	CreateManagersAndFactories();
	LoadManagerGuard loadManagerGuard;



	CShopStorage::GetInstance().LoadStorage("Json/Hats/HatBluePrints.json");

	MODELCOMP_MGR.SetScene(myScene);
	myScene->SetSkybox("skybox.dds");
	LoadManager::GetInstance().SetCurrentPlayState(this);
	LoadManager::GetInstance().SetCurrentScene(myScene);


	Lights::SDirectionalLight dirLight;
	dirLight.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	dirLight.direction = { 0.0f, 0.0f, 1.0f, 1.0f };
	myScene->AddDirectionalLight(dirLight);

	CONSOLE->GetLuaFunctions();

	myScene->AddCamera(CScene::eCameraType::ePlayerOneCamera);
	CU::Camera& playerCamera = myScene->GetCamera(CScene::eCameraType::ePlayerOneCamera);
	playerCamera.Init(50, WINDOW_SIZE_F.x, WINDOW_SIZE_F.y, 1.f, 75000.0f);

	SkillFactory::GetInstance().RegisterSkills();

	CCameraComponent* cameraComponent = CCameraComponentManager::GetInstance().CreateCameraComponent();
	cameraComponent->SetCamera(myScene->GetCamera(CScene::eCameraType::ePlayerOneCamera));

	myGoldText = new CTextInstance;
	myGoldText->SetColor(CTextInstance::Yellow);
	myGoldText->SetPosition(CU::Vector2f(0.4f, 0.2f));
	myGoldText->SetText("");
	myGoldText->Init();

	//Loading
	CU::CJsonValue levelsFile;
	const std::string& errorString = levelsFile.Parse("Json/LevelList.json");

	CU::CJsonValue levelsArray = levelsFile.at("levels");

#ifdef _DEBUG
	myLevelIndex = levelsArray.Size()-1;
#else
	const int levelIndex = 0;
#endif

	std::string levelPath = "Json/Levels/";
	levelPath += levelsArray[myLevelIndex].GetString();
	levelPath += "/LevelData.json";

	std::string questPath = "Json/Quests/";
	questPath += levelsArray[myLevelIndex].GetString();
	questPath += ".json";

	std::string navmeshPath = "Json/Levels/";
	navmeshPath += levelsArray[myLevelIndex].GetString();
	navmeshPath += "/Navmesh.obj";

	//NAVMESH
	std::ifstream infile(navmeshPath);
	if (infile.good())
	{
		myNavmesh.LoadFromFile(navmeshPath.c_str());
		PollingStation::Navmesh = &myNavmesh;
	}
	else
	{
		PollingStation::Navmesh = nullptr;
	}




	myQuestManager.LoadQuestlines(questPath);
	myQuestManager.CompleteEvent();
	KLoader::CKevinLoader &loader = KLoader::CKevinLoader::GetInstance();

	const KLoader::eError loadError = loader.LoadFile(levelPath);
	if (loadError != KLoader::eError::NO_LOADER_ERROR)
	{
		DL_ASSERT("Loading Failed");
	}
	if (PollingStation::PlayerInput != nullptr)
	{
		PollingStation::playerObject = PollingStation::PlayerInput->GetParent();

		//Dísclaimer: fult men funkar //lägg till allt spelar specifikt som inte LD behöver störas av här
		CPlayerHealthMessenger* healthMessenger = new CPlayerHealthMessenger();

		RespawnComponent* respawn = RespawnComponentManager::GetInstance().CreateAndRegisterComponent();

		CComponentManager::GetInstance().RegisterComponent(healthMessenger);

		PollingStation::playerObject->AddComponent(healthMessenger);
		PollingStation::playerObject->AddComponent(respawn);
		PollingStation::playerObject->AddComponent(new CPlayerHealthMessenger());
		PollingStation::playerObject->AddComponent(new CPlayerManaMessenger());
		PollingStation::playerObject->AddComponent(CPickupManager::GetInstance().CreatePickerUpperComp());

		PollingStation::playerObject->NotifyComponents(eComponentMessageType::eInit, SComponentMessageData());
	}
	myGameObjectManager->SendObjectsDoneMessage();





	CGameObject* mouseObject = myGameObjectManager->CreateGameObject();
	mouseObject->SetName("MouseObject");

	Intersection::CollisionData mouseCollisionData;
	mouseCollisionData.myPointData = new Intersection::SPoint();
	CCollisionComponent* mouseCollisionComponent = myCollisionComponentManager->CreateCollisionComponent(CCollisionComponentManager::eColliderType::ePoint, mouseCollisionData);
	mouseCollisionComponent->AddCollidsWith(eColliderType_Enemy | eColliderType_Player);
	mouseCollisionComponent->SetColliderType(eColliderType_Mouse);
	mouseObject->AddComponent(mouseCollisionComponent);
	myMouseComponent = new CMouseComponent(myScene->GetCamera(CScene::eCameraType::ePlayerOneCamera));
	mouseObject->AddComponent(myMouseComponent);

	if (PollingStation::playerObject != nullptr)
	{
		PollingStation::playerObject->AddComponent(new CPlayerHealthMessenger());
		PollingStation::playerObject->AddComponent(new CPlayerManaMessenger());
	}
	
	myHatMaker->LoadBluePrints("Json/Hats/HatBluePrints.json");

	myIsLoaded = true;

	//get time to load the level:
	timerMgr.UpdateTimers();
	float time = timerMgr.GetTimer(handle).GetLifeTime().GetMilliseconds();
	GAMEPLAY_LOG("Game Inited in %f ms", time);
}

void CPlayState::Init()
{
	//skillnad på load, init & konstructor ?
}

eStateStatus CPlayState::Update(const CU::Time& aDeltaTime)
{
	Audio::CAudioInterface* audio = Audio::CAudioInterface::GetInstance();
	if (audio != nullptr)
	{
		CAudioSourceComponentManager::GetInstance().Update();
	}

	if (PollingStation::playerData->myIsWhirlwinding == true)
	{
		audio->PostEvent("WhirlWind");
	}
	else
	{
		audio->PostEvent("StopWhirlWind");
	}

	CParticleEmitterComponentManager::GetInstance().UpdateEmitters(aDeltaTime);
	InputControllerManager::GetInstance().Update(aDeltaTime);
	MovementComponentManager::GetInstance().Update(aDeltaTime);
	AIControllerManager::GetInstance().Update(aDeltaTime);
	SkillSystemComponentManager::GetInstance().Update(aDeltaTime);
	CPickupManager::GetInstance().Update(aDeltaTime);
	RespawnComponentManager::GetInstance().Update(aDeltaTime);

	if (myGUIManager)
	{
		myGUIManager->Update(aDeltaTime);
	}

	myCollisionComponentManager->Update();
	myScene->Update(aDeltaTime);

	myGameObjectManager->DestroyObjectsWaitingForDestruction();
	std::string goldAmount = "Gold: ";
	goldAmount +=std::to_string(PollingStation::playerData->myGold);
	myGoldText->SetText(goldAmount.c_str());

	SkillComponentManager::GetInstance().Update(aDeltaTime);
	DropComponentManager::GetInstance().Update(aDeltaTime);

	myHealthBarManager->Update();

	return myStatus;
}

void CPlayState::Render()
{
	myScene->Render();

	myCollisionComponentManager->Render();

	SChangeStatesMessage msg;
	

	if (myShuldRenderNavmesh == true)
	{
		msg.myBlendState = eBlendState::eAlphaBlend;
		msg.myDepthStencilState = eDepthStencilState::eDisableDepth;
		msg.myRasterizerState = eRasterizerState::eWireFrame;
		msg.mySamplerState = eSamplerState::eClamp;

		RENDERER.AddRenderMessage(new SChangeStatesMessage(msg));
		myNavmesh.Render();
	}

	msg.myBlendState = eBlendState::eAlphaBlend;
	msg.myDepthStencilState = eDepthStencilState::eDisableDepth;
	msg.myRasterizerState = eRasterizerState::eDefault;
	msg.mySamplerState = eSamplerState::eClamp;
	RENDERER.AddRenderMessage(new SChangeStatesMessage(msg));

	myGUIManager->Render();

	msg.myBlendState = eBlendState::eNoBlend;
	msg.myDepthStencilState = eDepthStencilState::eDisableDepth;
	msg.myRasterizerState = eRasterizerState::eNoCulling;
	msg.mySamplerState = eSamplerState::eClamp;
	RENDERER.AddRenderMessage(new SChangeStatesMessage(msg));

	myHealthBarManager->Render();
	myGoldText->Render();

	myQuestDrawer.Render();
}

void CPlayState::OnEnter()
{
	PostMaster::GetInstance().Subscribe(this, eMessageType::eKeyboardMessage);
	Audio::CAudioInterface::GetInstance()->LoadBank("Audio/playState.bnk");
	Audio::CAudioInterface::GetInstance()->PostEvent("PlayCoolSong");
	myGUIManager->RestartRenderAndUpdate();
}

void CPlayState::OnExit()
{
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eKeyboardMessage);

	Audio::CAudioInterface* audioInterface = Audio::CAudioInterface::GetInstance();
	if (audioInterface != nullptr)
	{
		audioInterface->PostEvent("switchBank");
		audioInterface->PostEvent("StopCoolSong");
		audioInterface->UnLoadBank("Audio/playState.bnk");
	}
	myGUIManager->PauseRenderAndUpdate();
}

void CPlayState::Pause()
{
	PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::ePauseScreen, -1)));
}

void CPlayState::BuyHats()
{
	PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::eHatShop, -1)));
}

void CPlayState::CheckReturnToLevelSelect() // Formerly NextLevel -Kyle
{
	if (myShouldReturnToLevelSelect == true)
	{
		PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PopCurrentState()));
		//myStatus = eStateStatus::ePop; ?? //Was this only checked when pressing F7 - for change level ?
	}
}

eMessageReturn CPlayState::Recieve(const Message& aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}

CGameObjectManager* CPlayState::GetObjectManager() const
{
	return myGameObjectManager;
}

CHealthBarComponentManager * CPlayState::GetHealthBarManager()
{
	return myHealthBarManager;
}

CCollisionComponentManager* CPlayState::GetCollisionManager()
{
	return myCollisionComponentManager;
}

void CPlayState::CreateManagersAndFactories()
{
	myScene = new CScene();
	myGameObjectManager = new CGameObjectManager();
	myGUIManager = new GUI::GUIManager();
	myGUIManager->Init("Models/gui/guiBase.fbx");

	myCollisionComponentManager = new CCollisionComponentManager;
	CComponentManager::CreateInstance();
	CAudioSourceComponentManager::Create();
	CModelComponentManager::Create();
	CParticleEmitterComponentManager::Create();
	CParticleEmitterComponentManager::GetInstance().SetScene(myScene);
	CShopStorage::Create();
	CCameraComponentManager::Create();
	InputControllerManager::CreateInstance();
	InputControllerManager::GetInstance().SetScene(myScene);
	MovementComponentManager::CreateInstance();
	AIControllerManager::Create();
	FleeControllerManager::Create();
	CSeekControllerManager::Create();
	SkillFactory::CreateInstance();
	SkillSystemComponentManager::CreateInstance();
	SkillSystemComponentManager::GetInstance().SetGameObjectManager(myGameObjectManager);
	SkillSystemComponentManager::GetInstance().SetCollisionComponentManager(myCollisionComponentManager);
	SkillComponentManager::CreateInstance();
	DropComponentManager::CreateInstance();
	myHealthBarManager = new CHealthBarComponentManager(myScene->GetCamera(CScene::eCameraType::ePlayerOneCamera));
	ManaComponentManager::CreateInstance();
	myHatMaker = new CHatMaker(myGameObjectManager);
	CPickupFactory::Create(myGameObjectManager, myCollisionComponentManager);
	CPickupManager::CreateInstance();
	CMasterAI::Create();
	RespawnComponentManager::Create();
	CLevelManager::CreateInstance();
	CEnemyFactory::Create(*myGameObjectManager,*myCollisionComponentManager,*myHealthBarManager);
}
