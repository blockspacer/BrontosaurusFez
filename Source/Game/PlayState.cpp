#include "stdafx.h"
#include "PlayState.h"
#include "Game.h"
#include <Engine.h> //fixa camera instance sen
#include <CommonUtilities.h>
#include <Lights.h>
#include <Time.h>
#include <TimerManager.h>
#include <Renderer.h>
#include "Skybox.h"

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

//Kanske Inte ska vara här?
#include "../BrontosaurusEngine/Console.h"
#include "AIControllerManager.h"
//

//Temp Includes
#include "Components/InputController.h"
#include "Components/NavigationComponent.h"
#include "Components/MovementComponent.h"
#include "Components/HealthComponent.h"
#include "CameraComponent.h"
#include "BrontosaurusEngine/ModelInstance.h"
#include "BrontosaurusEngine/WindowsWindow.h"
#include <iostream>
#include "StatComponent.h"
#include "Components/AIControllerComponent.h"
#include "Components/ChaserController.h"

CPlayState::CPlayState(StateStack& aStateStack, const int aLevelIndex, const bool aShouldReturnToLevelSelect)
	: State(aStateStack)
	, myLevelIndex(aLevelIndex)
	, myShouldReturnToLevelSelect(aShouldReturnToLevelSelect)
{
	myIsLoaded = false;
}

static CModelInstance* animation = nullptr;
CPlayState::~CPlayState()
{
	SAFE_DELETE(myGameObjectManager);
	SAFE_DELETE(myGUIManager);
	CPhysicsManager::DestroyInstance();
	
	LoadManager::DestroyInstance();
	
	CPhysicsManager::DestroyInstance();
	CModelComponentManager::Destroy();
	CAudioSourceComponentManager::Destroy();
	CParticleEmitterComponentManager::Destroy();
	CCameraComponentManager::Destroy();
	InputControllerManager::DestroyInstance();
	MovementComponentManager::DestroyInstance();
	AIControllerManager::Destroy();

	PollingStation::NullifyLevelSpecificData();

	CComponentManager::DestroyInstance();
}

void CPlayState::Load()
{
	//start taking the time for loading level
	CU::TimerManager timerMgr;
	CU::TimerHandle handle = timerMgr.CreateTimer();
	timerMgr.StartTimer(handle);
	CreateManagersAndFactories();

	MODELCOMP_MGR.SetScene(&myScene);
	myScene.SetSkybox("skybox.dds");


	Lights::SDirectionalLight dirLight;
	dirLight.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	dirLight.direction = { 0.0f, 0.0f, 1.0f, 1.0f };
	myScene.AddDirectionalLight(dirLight);

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


	//create player:
	CGameObject* playerObject = myGameObjectManager->CreateGameObject();
	PollingStation::playerObject = playerObject;

	InputController* tempInputController = new InputController();
	InputControllerManager::GetInstance().RegisterComponent(tempInputController);
	playerObject->AddComponent(tempInputController);

	MovementComponent* tempMovementController = new MovementComponent();
	MovementComponentManager::GetInstance().RegisterComponent(tempMovementController);
	playerObject->AddComponent(tempMovementController);

	playerObject->AddComponent(new NavigationComponent());

	CModelComponent* playerModelComponent = CModelComponentManager::GetInstance().CreateComponent("Models/Player/player_idle.fbx");
	playerObject->AddComponent(playerModelComponent);

	playerObject->GetLocalTransform().SetPosition(CU::Vector3f(0.0f, 0.0f, 0.0f));

	//__TEMP____CREATE_AND_ADD_HAT_TO_PLAYER______

	CInventoryComponent inventoryComponent;
	playerObject->AddComponent(new CInventoryComponent());

	//__TEMP______________________________________

	//create camera object:
	//myCameraObject = myGameObjectManager->CreateGameObject();

	myScene.AddCamera(CScene::eCameraType::ePlayerOneCamera);
	CCameraComponent* cameraComponent = CCameraComponentManager::GetInstance().CreateCameraComponent();
	cameraComponent->SetCamera(myScene.GetCamera(CScene::eCameraType::ePlayerOneCamera));

	//myCameraObject->GetLocalTransform().SetPosition(CU::Vector3f(0.0f, 0.0f, -100.0f));

	//CU::Matrix33f camerarotationMatrix = myCameraObject->GetLocalTransform().GetRotation();
	//camerarotationMatrix.LookAt(myCameraObject->GetWorlPosition(), playerObject->GetWorlPosition());
	//myCameraObject->GetLocalTransform().SetRotation(camerarotationMatrix);
	//playerObject->AddComponent(myCameraObject);


	//set camera position and rotation
	CU::Camera& playerCamera = myScene.GetCamera(CScene::eCameraType::ePlayerOneCamera);
	playerCamera.Init(60, WINDOW_SIZE.x, WINDOW_SIZE.y, 1.f, 75000.0f, { 0.0f, 0.0f, 0.f });

	CU::Matrix44f cameraTransformation = playerCamera.GetTransformation();
	CU::Matrix44f newRotation;

	newRotation.Rotate(PI / 2, CU::Axees::X);
	newRotation.Rotate(PI / 4, CU::Axees::X);
	newRotation.Rotate(PI / 1, CU::Axees::Z);

	cameraTransformation.SetRotation(newRotation);
	cameraTransformation.SetPosition(CU::Vector3f(0.0f, 0.0f, 0.0f));
	cameraTransformation.Move(CU::Vector3f(000.0f, 000.0f, -1500.0f));

	playerCamera.SetTransformation(cameraTransformation);
	cameraComponent->InitOffsetPosition();

	//myCameraObject->GetLocalTransform() = cameraTransformation;
	//myCameraObject->NotifyComponents(eComponentMessageType::eMoving, SComponentMessageData());
	//myCameraObject->AddComponent(cameraComponent);
	playerObject->AddComponent(cameraComponent);

	//CAMERA->SetTransformation(CCameraComponentManager::GetInstance().GetActiveCamera().GetTransformation());
	//----MakeEnemy----
	CGameObject* TempraryEnemyObject = myGameObjectManager->CreateGameObject();
	CModelComponent* tempEnemyModel = CModelComponentManager::GetInstance().CreateComponent("Models/Placeholders/tree.fbx");
	CStatComponent* tempEnemyStatComponent = new CStatComponent();
	tempEnemyStatComponent->Set(1, 1, 1, 1);
	CHealthComponent* tempEnemyHealthComponent = new CHealthComponent();

	CAIControllerComponent* AIController = new CAIControllerComponent();
	CChaserController* chaserController = new CChaserController();
	chaserController->SetMaxAcceleration(1);
	chaserController->SetMaxSpeed(10);
	chaserController->SetSlowDownRadius(100);
	chaserController->SetTargetRadius(100);

	TempraryEnemyObject->AddComponent(AIController);
	AIController->AddControllerBehaviour(chaserController);

	AIControllerManager::GetIstance().AddController(AIController);

	TempraryEnemyObject->AddComponent(tempEnemyModel);
	TempraryEnemyObject->AddComponent(tempEnemyStatComponent);
	TempraryEnemyObject->AddComponent(tempEnemyHealthComponent);

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
	//skillnad på load, init & konstructor ?
}

State::eStatus CPlayState::Update(const CU::Time& aDeltaTime)
{
	//CU::Matrix44f cameraTransformation = myScene.GetCamera(CScene::eCameraType::ePlayerOneCamera).GetTransformation();
	//CU::Matrix44f newRotation;

	//newRotation.Rotate(PI / 2, CU::Axees::X);
	//newRotation.Rotate(PI / 4, CU::Axees::X);
	//newRotation.Rotate(PI / 1, CU::Axees::Z);

	//cameraTransformation.SetRotation(newRotation);
	//cameraTransformation.SetPosition(CU::Vector3f(0.0f, 0.0f, 0.0f));
	//cameraTransformation.Move(CU::Vector3f(000.0f, 000.0f, -1500.0f));


	//myCameraObject->GetLocalTransform() = cameraTransformation;
	//myScene.GetCamera(CScene::eCameraType::ePlayerOneCamera).SetTransformation(CCameraComponentManager::GetInstance().GetActiveCamera().GetTransformation());


	//myGUIManager->Update(aDeltaTime)
	
	
	Audio::CAudioInterface* audio = Audio::CAudioInterface::GetInstance();
	if (audio != nullptr)
	{
		CAudioSourceComponentManager::GetInstance().Update();
	}

	CPhysicsManager::GetInstance().Update();

	
	CParticleEmitterComponentManager::GetInstance().UpdateEmitters(aDeltaTime);
	InputControllerManager::GetInstance().Update(aDeltaTime);
	MovementComponentManager::GetInstance().Update(aDeltaTime);
	AIControllerManager::GetIstance().Update(aDeltaTime);
	myScene.Update(aDeltaTime);

	myGameObjectManager->DestroyObjectsWaitingForDestruction();

	return myStatus;
}

void CPlayState::Render()
{
	myScene.Render();

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

void CPlayState::CreateManagersAndFactories()
{
	myGameObjectManager = new CGameObjectManager();
	myGUIManager = new GUI::GUIManager();
	//myGUIManager->Init("Models/gui/gui.fbx", true);

	CComponentManager::CreateInstance();
	CPhysicsManager::CreateInstance();
	LoadManager::CreateInstance();
	CAudioSourceComponentManager::Create();
	CModelComponentManager::Create();
	CParticleEmitterComponentManager::Create();
	CParticleEmitterComponentManager::GetInstance().SetScene(&myScene);
	CCameraComponentManager::Create();
	InputControllerManager::CreateInstance();
	MovementComponentManager::CreateInstance();
	AIControllerManager::Create();
}
