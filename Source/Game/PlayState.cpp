#include "stdafx.h"
#include "PlayState.h"

#include <Engine.h> //fixa camera instance sen
#include <CommonUtilities.h>
#include <Lights.h>
#include <Time.h>
#include <TimerManager.h>
#include <Renderer.h>
#include "Skybox.h"
#include <Scene.h>

#include "Components/GameObjectManager.h"
#include "Components/AudioSourceComponentManager.h"
#include "Components/ModelComponentManager.h"
#include "Components/GameObject.h"
#include "Components/ModelComponent.h"
#include "Components/ModelComponentManager.h"
#include "Components/ParticleEmitterComponentManager.h"
#include "Components/ComponentManager.h"
#include "Components/InventoryComponent.h"

#include "PostMaster/PopCurrentState.h"
#include "PostMaster/ChangeLevel.h"
#include "PostMaster/PostMaster.h"
#include "PostMaster/PushState.h"
#include "PostMaster/Message.h"
#include "PostMaster/Event.h"

#include "PollingStation.h"
#include "CameraManager.h"
#include "InputControllerManager.h"
#include "MovementComponentManager.h"

#include "BrontosaurusEngine/LineDrawer.h"
#include "BrontosaurusEngine/TextInstance.h"

#include "../LuaWrapper/SSlua/SSlua.h"

#include "../GUI/GUIManager/GUIManager.h"

#include "LoadManager/LoadManager.h"

#include "../Audio/AudioInterface.h"

//Kanske Inte ska vara h�r?
#include "../BrontosaurusEngine/Console.h"
//

//Temp Includes
#include "Components/InputController.h"
#include "Components/NavigationComponent.h"
#include "Components/MovementComponent.h"
#include "Components/HealthComponent.h"
#include "MainStatComponent.h"
#include "StatComponent.h"
#include "CameraComponent.h"
#include "BrontosaurusEngine/ModelInstance.h"
#include "BrontosaurusEngine/WindowsWindow.h"

#include "Components\SkillFactory.h"
#include "StatComponent.h"

CPlayState::CPlayState(StateStack& aStateStack, const int aLevelIndex, const bool aShouldReturnToLevelSelect)
	: State(aStateStack)
	, myLevelIndex(aLevelIndex)
	, myShouldReturnToLevelSelect(aShouldReturnToLevelSelect)
	, myScene(nullptr)
{
	myIsLoaded = false;
}

CPlayState::~CPlayState()
{
	SAFE_DELETE(myScene);
	SAFE_DELETE(myGameObjectManager);
	SAFE_DELETE(myGUIManager);
	
	LoadManager::DestroyInstance();
	
	CModelComponentManager::Destroy();
	CAudioSourceComponentManager::Destroy();
	CParticleEmitterComponentManager::Destroy();
	CCameraComponentManager::Destroy();
	InputControllerManager::DestroyInstance();
	MovementComponentManager::DestroyInstance();

	PollingStation::NullifyLevelSpecificData();

	CComponentManager::DestroyInstance();
	SkillFactory::DestroyInstance();
}

void CPlayState::Load()
{
	//start taking the time for loading level
	CU::TimerManager timerMgr;
	CU::TimerHandle handle = timerMgr.CreateTimer();
	timerMgr.StartTimer(handle);
	CreateManagersAndFactories();

	MODELCOMP_MGR.SetScene(myScene);
	myScene->SetSkybox("skybox.dds");


	Lights::SDirectionalLight dirLight;
	dirLight.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	dirLight.direction = { 0.0f, 0.0f, 1.0f, 1.0f };
	myScene->AddDirectionalLight(dirLight);

	//LOAD_MGR.SetCurrentScene(&myScene);
	//LOAD_MGR.SetCurrentPlayState(this);

	//SSlua::ArgumentList levelIndex(1);
	//levelIndex.Add(SSArgument(ssLuaNumber(myLevelIndex)));
	//LUA_WRAPPER.CallLuaFunction("GameLoad", levelIndex);

	//kanske inte ska ske s� h�r?
	LUA_WRAPPER.RegisterFunction(SSlua::LuaCallbackFunction(&CPlayState::LuaFunction), "Func", "lol", true);
	CONSOLE->GetLuaFunctions();
	//


	//hue hue dags att fula ner play state - Alex(Absolut inte Marcus); // sn�lla sl� Johan inte mig(Alex);

	//create an npc
	CGameObject* npcObject1 = myGameObjectManager->CreateGameObject();
	npcObject1->GetLocalTransform().Move(CU::Vector3f(0.0f, 000.0f, 500.0f));

	CModelComponent* modelComponent1 = CModelComponentManager::GetInstance().CreateComponent("Models/Player/player_idle.fbx");
	npcObject1->AddComponent(modelComponent1);


	//create another npc
	CGameObject* npcObject2 = myGameObjectManager->CreateGameObject();
	npcObject2->GetLocalTransform().Move(CU::Vector3f(500.0f, 0.0f, 0.0f));

	CModelComponent* modelComponent2= CModelComponentManager::GetInstance().CreateComponent("Models/Player/player_idle.fbx");
	npcObject2->AddComponent(modelComponent2);


	myScene->AddCamera(CScene::eCameraType::ePlayerOneCamera);
	CU::Camera& playerCamera = myScene->GetCamera(CScene::eCameraType::ePlayerOneCamera);
	playerCamera.Init(60, WINDOW_SIZE.x, WINDOW_SIZE.y, 1.f, 75000.0f, { 0.0f, 0.0f, 0.f });

	//create player:
	myPlayerObject = myGameObjectManager->CreateGameObject();

	InputController* tempInputController = new InputController(playerCamera);
	InputControllerManager::GetInstance().RegisterComponent(tempInputController);
	myPlayerObject->AddComponent(tempInputController);

	MovementComponent* tempMovementController = new MovementComponent();
	MovementComponentManager::GetInstance().RegisterComponent(tempMovementController);
	myPlayerObject->AddComponent(tempMovementController);

	myPlayerObject->AddComponent(new NavigationComponent());

	CModelComponent* playerModelComponent = CModelComponentManager::GetInstance().CreateComponent("Models/Player/player_idle.fbx");
	myPlayerObject->AddComponent(playerModelComponent);

	myPlayerObject->GetLocalTransform().SetPosition(CU::Vector3f(0.0f, 0.0f, 0.0f));

	//create camera component:
	CCameraComponent* cameraComponent = CCameraComponentManager::GetInstance().CreateCameraComponent();
	cameraComponent->SetCamera(myScene->GetCamera(CScene::eCameraType::ePlayerOneCamera));

	//myCameraObject->GetLocalTransform().SetPosition(CU::Vector3f(0.0f, 0.0f, -100.0f));

	//CU::Matrix33f camerarotationMatrix = myCameraObject->GetLocalTransform().GetRotation();
	//camerarotationMatrix.LookAt(myCameraObject->GetWorlPosition(), playerObject->GetWorlPosition());
	//myCameraObject->GetLocalTransform().SetRotation(camerarotationMatrix);
	//playerObject->AddComponent(myCameraObject);


	//set camera position and rotation
	CU::Matrix44f cameraTransformation = playerCamera.GetTransformation();
	CU::Matrix44f newRotation;

	newRotation.Rotate(PI / 2, CU::Axees::X);
	newRotation.Rotate(PI / 4, CU::Axees::X);
	newRotation.Rotate(PI / 1, CU::Axees::Z);

	cameraTransformation.SetRotation(newRotation);
	cameraTransformation.SetPosition(CU::Vector3f(0.0f, 0.0f, 0.0f));
	cameraTransformation.Move(CU::Vector3f(0.0f, 0.0f, -1100.0f));

	playerCamera.SetTransformation(cameraTransformation);
	cameraComponent->InitOffsetPosition();

	myPlayerObject->AddComponent(cameraComponent);

	//CAMERA->SetTransformation(CCameraComponentManager::GetInstance().GetActiveCamera().GetTransformation());
	//----MakeEnemy----
	CGameObject* TempraryEnemyObject = myGameObjectManager->CreateGameObject();
	CModelComponent* tempEnemyModel = CModelComponentManager::GetInstance().CreateComponent("Models/Placeholders/tree.fbx");
	
	Stats::SBaseStats baseStats;
	baseStats.Dexterity = 1337;
	Stats::SBonusStats bonusStats;
	CStatComponent* tempEnemyStatComponent = new CStatComponent();
	CHealthComponent* tempEnemyHealthComponent = new CHealthComponent();

	TempraryEnemyObject->AddComponent(tempEnemyModel);
	TempraryEnemyObject->AddComponent(tempEnemyStatComponent);
	TempraryEnemyObject->AddComponent(tempEnemyHealthComponent);

	tempEnemyStatComponent->SetStats(baseStats, bonusStats);

	tempEnemyHealthComponent->Init();

	//-----------------

	
	myIsLoaded = true;

	//get time to load the level:
	timerMgr.UpdateTimers();
	float time = timerMgr.GetTimer(handle).GetLifeTime().GetMilliseconds();
	GAMEPLAY_LOG("Game Inited in %f ms", time);
}

void CPlayState::Init()
{
	//skillnad p� load, init & konstructor ?
}

State::eStatus CPlayState::Update(const CU::Time& aDeltaTime)
{
	Audio::CAudioInterface* audio = Audio::CAudioInterface::GetInstance();
	if (audio != nullptr)
	{
		CAudioSourceComponentManager::GetInstance().Update();
	}
	
	CParticleEmitterComponentManager::GetInstance().UpdateEmitters(aDeltaTime);
	InputControllerManager::GetInstance().Update(aDeltaTime);
	MovementComponentManager::GetInstance().Update(aDeltaTime);
	myScene->Update(aDeltaTime);

	myGameObjectManager->DestroyObjectsWaitingForDestruction();

	return myStatus;
}

void CPlayState::Render()
{
	myScene->Render();

	SChangeStatesMessage msg;
	msg.myBlendState = eBlendState::eAlphaBlend;
	msg.myDepthStencilState = eDepthStencilState::eDefault;
	msg.myRasterizerState = eRasterizerState::eNoCulling;
	msg.mySamplerState = eSamplerState::eClamp;
	
	
	RENDERER.AddRenderMessage(new SChangeStatesMessage(msg));

	msg.myBlendState = eBlendState::eAlphaBlend;
	msg.myDepthStencilState = eDepthStencilState::eDisableDepth;
	msg.myRasterizerState = eRasterizerState::eDefault;
	msg.mySamplerState = eSamplerState::eClamp;
	RENDERER.AddRenderMessage(new SChangeStatesMessage(msg));

	//myGUIManager->Render();

	msg.myBlendState = eBlendState::eNoBlend;
	msg.myDepthStencilState = eDepthStencilState::eDefault;
	msg.myRasterizerState = eRasterizerState::eDefault;
	msg.mySamplerState = eSamplerState::eClamp;
	RENDERER.AddRenderMessage(new SChangeStatesMessage(msg));
}

void CPlayState::OnEnter()
{
	//PostMaster::GetInstance().AppendSubscriber(this, eMessageType::eStateMessage);
	PostMaster::GetInstance().AppendSubscriber(this, eMessageType::eKeyboardMessage);
	PostMaster::GetInstance().AppendSubscriber(this, eMessageType::eNextLevelPlease);
	Audio::CAudioInterface::GetInstance()->LoadBank("Audio/playState.bnk");
	Audio::CAudioInterface::GetInstance()->PostEvent("PlayCoolSong");
	//Audio::CAudioInterface::GetInstance()->PostEvent("PlayerMoving_Play");
	//myGUIManager->RestartRenderAndUpdate();
}

void CPlayState::OnExit()
{
	//PostMaster::GetInstance().UnSubscribe(this, eMessageType::eStateMessage);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eKeyboardMessage);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eNextLevelPlease);

	Audio::CAudioInterface* audioInterface = Audio::CAudioInterface::GetInstance();
	if (audioInterface != nullptr)
	{
		audioInterface->PostEvent("switchBank");
		audioInterface->PostEvent("StopCoolSong");
		//audioInterface->PostEvent("PlayerMoving_Stop");
		audioInterface->UnLoadBank("Audio/playState.bnk");
	}
	myGUIManager->PauseRenderAndUpdate();
}

void CPlayState::Pause()
{
	PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::ePauseScreen, -1)));
}

void CPlayState::NextLevel()
{
	if (myShouldReturnToLevelSelect == true)
	{
		PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PopCurrentState()));
		//myStatus = eStateStatus::ePop; ??
	}
	else
	{
		int levelIndex = (myLevelIndex > 0) ? myLevelIndex + 1 : 2; //this is ugly and was just for fun, sorry about that mvh Carl
		PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, ChangeLevel(levelIndex)));
	}
}

eMessageReturn CPlayState::Recieve(const Message& aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}


void CPlayState::TEMP_ADD_HAT(CGameObject * aPlayerObject)
{
	//__TEMP____CREATES_AND_ADDS_HAT_TO_PLAYER_OBJ_____

	CGameObject* hatObj = myGameObjectManager->CreateGameObject();
	CModelComponent* hatModel = MODELCOMP_MGR.CreateComponent("Models/Player/hat_basic.fbx");
	hatObj->AddComponent(hatModel);
	CU::Vector3f hatPos = hatObj->GetLocalTransform().GetPosition();
	hatObj->GetLocalTransform().SetPosition({ hatPos.x, hatPos.y + 175.f, hatPos.z });
	aPlayerObject->AddComponent(hatObj);

	CMainStatComponent* mainStat = new CMainStatComponent;
	aPlayerObject->AddComponent(mainStat);

	CStatComponent* stat1 = new CStatComponent;
	CStatComponent* stat2 = new CStatComponent;
	CStatComponent* stat3 = new CStatComponent;


	hatObj->AddComponent(stat1);
	aPlayerObject->AddComponent(stat2);
	hatObj->AddComponent(stat3);

	Stats::SBaseStats base1;
	base1.Dexterity = 1;
	base1.Intelligence = 1;
	base1.Strength = 1;
	base1.Vitality = 1;
	Stats::SBonusStats bonus1;
	bonus1.BonusArmor = 1;
	bonus1.BonusCritChance = 1;
	bonus1.BonusCritDamage = 1;
	bonus1.BonusDamage = 1;
	bonus1.BonusHealth = 1;
	bonus1.BonusMovementSpeed = 1;
	stat1->SetStats(base1, bonus1);

	Stats::SBaseStats base2;
	base2.Dexterity = 2;
	base2.Intelligence = 2;
	base2.Strength = 2;
	base2.Vitality = 2;
	Stats::SBonusStats bonus2;
	bonus2.BonusArmor = 2;
	bonus2.BonusCritChance = 2;
	bonus2.BonusCritDamage = 2;
	bonus2.BonusDamage = 2;
	bonus2.BonusHealth = 2;
	bonus2.BonusMovementSpeed = 2;
	stat2->SetStats(base2, bonus2);

	Stats::SBaseStats base3;
	base3.Dexterity = 3;
	base3.Intelligence = 3;
	base3.Strength = 3;
	base3.Vitality = 3;
	Stats::SBonusStats bonus3;
	bonus3.BonusArmor = 3;
	bonus3.BonusCritChance = 3;
	bonus3.BonusCritDamage = 3;
	bonus3.BonusDamage = 3;
	bonus3.BonusHealth = 3;
	bonus3.BonusMovementSpeed = 3;
	stat3->SetStats(base3, bonus3);
}

void CPlayState::CreateManagersAndFactories()
{
	myScene = new CScene();
	myGameObjectManager = new CGameObjectManager();
	myGUIManager = new GUI::GUIManager();
	//myGUIManager->Init("Models/gui/gui.fbx", true);

	CComponentManager::CreateInstance();
	LoadManager::CreateInstance();
	CAudioSourceComponentManager::Create();
	CModelComponentManager::Create();
	CParticleEmitterComponentManager::Create();
	CParticleEmitterComponentManager::GetInstance().SetScene(myScene);
	CCameraComponentManager::Create();
	InputControllerManager::CreateInstance();
	MovementComponentManager::CreateInstance();
	SkillFactory::CreateInstance();
}
