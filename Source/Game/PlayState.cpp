#include "stdafx.h"
#include "PlayState.h"

#include <Engine.h> //fixa camera instance sen
#include <CommonUtilities.h>
#include <Lights.h>
#include <TimerManager.h>
#include <Renderer.h>
#include "Skybox.h"
#include <Scene.h>

#include "Components/GameObjectManager.h"
#include "Components/AudioSourceComponentManager.h"
#include "Components/ModelComponentManager.h"
#include "Components/GameObject.h"
#include "Components/ModelComponent.h"
#include "Components/ParticleEmitterComponentManager.h"
#include "Components/ComponentManager.h"

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

#include "Components\SkillFactory.h"
#include "Components\SkillSystemComponentManager.h"
//Kanske Inte ska vara här?
#include "../BrontosaurusEngine/Console.h"
#include "AIControllerManager.h"
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
#include <iostream>
#include "StatComponent.h"
#include "Components/AIControllerComponent.h"
#include "Components/SeekController.h"
#include "Components\FleeController.h"
#include "Components\SkillFactory.h"
#include "SkillSystemComponent.h"
#include "KevinLoader/KevinLoader.h"
#include "Components\CollisionComponentManager.h"

//ULTRA TEMP INCLUDES, remove if you see and remove the things that don't compile afterwards
#include "../BrontosaurusEngine/FireEmitterInstance.h"
#include "../BrontosaurusEngine/FireEmitterData.h"

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
	
	
	CModelComponentManager::Destroy();
	CAudioSourceComponentManager::Destroy();
	CParticleEmitterComponentManager::Destroy();
	CCameraComponentManager::Destroy();
	InputControllerManager::DestroyInstance();
	MovementComponentManager::DestroyInstance();
	SkillSystemComponentManager::DestroyInstance();
	AIControllerManager::Destroy();

	PollingStation::NullifyLevelSpecificData();

	SkillFactory::DestroyInstance();
	CComponentManager::DestroyInstance();

}

void CPlayState::Load()
{
	LoadManagerGuard loadManagerGuard;

	//start taking the time for loading level
	CU::TimerManager timerMgr;
	CU::TimerHandle handle = timerMgr.CreateTimer();
	timerMgr.StartTimer(handle);
	CreateManagersAndFactories();

	MODELCOMP_MGR.SetScene(myScene);
	myScene->SetSkybox("skybox.dds");
	LoadManager::GetInstance().SetCurrentPlayState(this);
	LoadManager::GetInstance().SetCurrentScene(myScene);

	Lights::SDirectionalLight dirLight;
	dirLight.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	dirLight.direction = { 0.0f, 0.0f, 1.0f, 1.0f };
	myScene->AddDirectionalLight(dirLight);

	//LOAD_MGR.SetCurrentScene(&myScene);
	//LOAD_MGR.SetCurrentPlayState(this);

	//SSlua::ArgumentList levelIndex(1);
	//levelIndex.Add(SSArgument(ssLuaNumber(myLevelIndex)));
	//LUA_WRAPPER.CallLuaFunction("GameLoad", levelIndex);

	//kanske inte ska ske så här?
	LUA_WRAPPER.RegisterFunction(SSlua::LuaCallbackFunction(&CPlayState::LuaFunction), "Func", "lol", true);
	CONSOLE->GetLuaFunctions();
	//


	//hue hue dags att fula ner play state - Alex(Absolut inte Marcus); // snälla slå Johan inte mig(Alex);

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

	//Move Later for modification from unity
	myScene->AddCamera(CScene::eCameraType::ePlayerOneCamera);
	CU::Camera& playerCamera = myScene->GetCamera(CScene::eCameraType::ePlayerOneCamera);
	playerCamera.Init(60, WINDOW_SIZE_F.x, WINDOW_SIZE_F.y, 1.f, 75000.0f, { 0.0f, 0.0f, 0.f });

	//create player:

	//myPlayerObject = myGameObjectManager->CreateGameObject();
	//PollingStation::playerObject = myPlayerObject;

	//InputController* tempInputController = new InputController(playerCamera);
	//InputControllerManager::GetInstance().RegisterComponent(tempInputController);
	//myPlayerObject->AddComponent(tempInputController);

	//MovementComponent* tempMovementController = new MovementComponent();
	//MovementComponentManager::GetInstance().RegisterComponent(tempMovementController);
	//myPlayerObject->AddComponent(tempMovementController);

	//myPlayerObject->AddComponent(new NavigationComponent());

	//CModelComponent* playerModelComponent = CModelComponentManager::GetInstance().CreateComponent("Models/Player/player_idle.fbx");
	//myPlayerObject->AddComponent(playerModelComponent);

	//myPlayerObject->GetLocalTransform().SetPosition(CU::Vector3f(0.0f, 0.0f, 0.0f));
	//SkillSystemComponent* tempSkillSystemComponent = new SkillSystemComponent;
	//SkillSystemComponentManager::GetInstance().RegisterComponent(tempSkillSystemComponent);
	//myPlayerObject->AddComponent(tempSkillSystemComponent);
	//tempSkillSystemComponent->AddSkill("BasicAttack");
	////create camera object:
	////myCameraObject = myGameObjectManager->CreateGameObject();

	//CCameraComponent* cameraComponent = CCameraComponentManager::GetInstance().CreateCameraComponent();
	//cameraComponent->SetCamera(myScene->GetCamera(CScene::eCameraType::ePlayerOneCamera));

	//myCameraObject->GetLocalTransform().SetPosition(CU::Vector3f(0.0f, 0.0f, -100.0f));

	//CU::Matrix33f camerarotationMatrix = myCameraObject->GetLocalTransform().GetRotation();
	//camerarotationMatrix.LookAt(myCameraObject->GetWorlPosition(), playerObject->GetWorlPosition());
	//myCameraObject->GetLocalTransform().SetRotation(camerarotationMatrix);
	//playerObject->AddComponent(myCameraObject);


	//set camera position and rotation
	/*CU::Matrix44f cameraTransformation = playerCamera.GetTransformation();
	CU::Matrix44f newRotation;

	newRotation.Rotate(PI / 4 * 4 , CU::Axees::Y);
	newRotation.Rotate(PI / 4, CU::Axees::X);

	//newRotation.Rotate(PI / 1, CU::Axees::Z);

	cameraTransformation.SetRotation(newRotation);
	cameraTransformation.SetPosition(CU::Vector3f(0.0f, 0.0f, 0.0f));
	cameraTransformation.Move(CU::Vector3f(0.0f, 0.0f, -1100.0f));

	playerCamera.SetTransformation(cameraTransformation);*/



	//Loadingu like pingu

	CU::CPJWrapper levelsFile;
	const std::string errorString = levelsFile.Parse("Json/LevelList.json");

	CU::CPJWrapper levelsArray = levelsFile.GetJsonObject().at("levels");

#ifdef _DEBUG
	const int levelIndex = levelsArray.Size() - 1;
#else
	const int levelIndex = 0;
#endif

	std::string levelPath = "Json/Levels/";
	levelPath += levelsArray[levelIndex].GetString();
	levelPath += "/LevelData.json";

	KLoader::CKevinLoader &loader = KLoader::CKevinLoader::GetInstance();

	const KLoader::eError loadError = loader.LoadFile(levelPath);
	if (loadError != KLoader::eError::NO_LOADER_ERROR)
	{
		DL_ASSERT("Loading Failed");
	}

	//cameraComponent->InitOffsetPosition();

	//myPlayerObject->AddComponent(cameraComponent);

	//CAMERA->SetTransformation(CCameraComponentManager::GetInstance().GetActiveCamera().GetTransformation()); //

	//----CreateEnemies----
	/*myEnemies.Init(8);
	TEMP_CREATE_ENEMY();
	myEnemies[0]->SetWorldPosition({ 0.f, 0.f, 0.f });
	TEMP_CREATE_ENEMY();
	myEnemies[1]->SetWorldPosition({ 300.f, 0.f, 0.f });
	TEMP_CREATE_ENEMY();
	myEnemies[2]->SetWorldPosition({ 0.f, 0.f, 800.f });*/

	//---------------------

	CFireEmitterInstance fireeeeeByCarl;
	SFireEmitterData fireData;
	fireData.myScrollSpeeds[0] = 1.3f;
	fireData.myScrollSpeeds[1] = 2.1f;
	fireData.myScrollSpeeds[2] = 2.3f;

	fireData.myScales[0] = 1.f;
	fireData.myScales[1] = 2.f;
	fireData.myScales[2] = 3.f;

	fireData.myDistortions[0].Set(0.1f, 0.2f);
	fireData.myDistortions[1].Set(0.1f, 0.3f);
	fireData.myDistortions[2].Set(0.1f, 0.1f);

	fireData.myDistortionScale = 0.8f;
	fireData.myDistortionBias = 0.5f;

	fireeeeeByCarl.Init(fireData);
	fireeeeeByCarl.GetTransformation().Rotate(3.141592f, CU::Axees::Y);
	fireeeeeByCarl.GetTransformation().m11 *= 2.f;
	fireeeeeByCarl.GetTransformation().m22 *= 2.f;
	fireeeeeByCarl.GetTransformation().m33 *= 2.f;
	myScene->AddFireEmitters(fireeeeeByCarl);
	
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
	AIControllerManager::GetInstance().Update(aDeltaTime);
	SkillSystemComponentManager::GetInstance().Update(aDeltaTime);
	myCollisionComponentManager->Update();
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
	//PostMaster::GetInstance().Subscribe(this, eMessageType::eStateMessage);
	PostMaster::GetInstance().Subscribe(this, eMessageType::eKeyboardMessage);
	PostMaster::GetInstance().Subscribe(this, eMessageType::eNextLevelPlease);
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

CGameObjectManager* CPlayState::GetObjectManager() const
{
	return myGameObjectManager;
}

void CPlayState::CreateManagersAndFactories()
{
	myScene = new CScene();
	myGameObjectManager = new CGameObjectManager();
	myGUIManager = new GUI::GUIManager();
	//myGUIManager->Init("Models/gui/gui.fbx", true);

	myCollisionComponentManager = new CCollisionComponentManager;
	CComponentManager::CreateInstance();
	CAudioSourceComponentManager::Create();
	CModelComponentManager::Create();
	CParticleEmitterComponentManager::Create();
	CParticleEmitterComponentManager::GetInstance().SetScene(myScene);
	CCameraComponentManager::Create();
	InputControllerManager::CreateInstance();
	MovementComponentManager::CreateInstance();
	AIControllerManager::Create();
	SkillFactory::CreateInstance();
	SkillSystemComponentManager::CreateInstance();
	SkillSystemComponentManager::GetInstance().SetGameObjectManager(myGameObjectManager);
	SkillSystemComponentManager::GetInstance().SetCollisionComponentManager(myCollisionComponentManager);
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


void CPlayState::TEMP_CREATE_ENEMY()
{
	CGameObject* enemyObj = myGameObjectManager->CreateGameObject();
	CModelComponent* tempEnemyModel = CModelComponentManager::GetInstance().CreateComponent("Models/Placeholders/tree.fbx");
	CStatComponent* tempEnemyStatComponent = new CStatComponent();
	CAIControllerComponent* AIController = new CAIControllerComponent();
	CHealthComponent* tempEnemyHealthComponent = new CHealthComponent();
	CSeekController* seekController = new CSeekController();
	CFleeController* fleeController = new CFleeController();

	enemyObj->AddComponent(tempEnemyModel);
	enemyObj->AddComponent(AIController);
	enemyObj->AddComponent(tempEnemyStatComponent);
	enemyObj->AddComponent(tempEnemyHealthComponent);

	AIController->AddControllerBehaviour(seekController);
	AIController->AddControllerBehaviour(fleeController);

	Stats::SBaseStats baseStats;
	baseStats.Dexterity = 1337;
	Stats::SBonusStats bonusStats;

	seekController->SetMaxAcceleration(400);
	seekController->SetMaxSpeed(30);
	seekController->SetSlowDownRadius(100);
	seekController->SetTargetRadius(70);

	fleeController->SetTargetsToAvoid(&myEnemies);

	AIControllerManager::GetInstance().AddController(AIController);

	tempEnemyStatComponent->SetStats(baseStats, bonusStats);
	tempEnemyHealthComponent->Init();

	myCollisionComponentManager = new CCollisionComponentManager;
	CComponentManager::CreateInstance();
	CAudioSourceComponentManager::Create();
	CModelComponentManager::Create();
	CParticleEmitterComponentManager::Create();
	CParticleEmitterComponentManager::GetInstance().SetScene(myScene);
	CCameraComponentManager::Create();
	InputControllerManager::CreateInstance();
	InputControllerManager::GetInstance().SetScene(myScene);
	MovementComponentManager::CreateInstance();
	AIControllerManager::Create();
	SkillFactory::CreateInstance();
	SkillSystemComponentManager::CreateInstance();
	SkillSystemComponentManager::GetInstance().SetGameObjectManager(myGameObjectManager);
	SkillSystemComponentManager::GetInstance().SetCollisionComponentManager(myCollisionComponentManager);
	myEnemies.Add(enemyObj);
}
