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

#include "../Components/AudioSourceComponentManager.h"
#include "../Components/ModelComponentManager.h"
#include "../Components/GameObject.h"
#include "../BrontosaurusEngine/LineDrawer.h"
#include "../LuaWrapper/SSlua/SSlua.h"
#include "../GUI/GUIManager/GUIManager.h"
#include "LoadManager/LoadManager.h"
#include "PollingStation.h"
#include "PostMaster/PostMaster.h"
#include "../Audio/AudioInterface.h"
#include "PostMaster/PushState.h"
#include "Components/ModelComponent.h"
#include "Components/ModelComponentManager.h"
#include "BrontosaurusEngine/TextInstance.h"
#include "../Components/ComponentManager.h"
#include "Components/ParticleEmitterComponentManager.h"
#include "PostMaster/ChangeLevel.h"
#include "PostMaster/PopCurrentState.h"
#include "PostMaster/Message.h"
#include "PostMaster/Event.h"
#include "CameraManager.h"
#include "InputControllerManager.h"
#include "MovementComponentManager.h"

//Temp Includes
#include "Components/InputController.h"
#include "Components/NavigationComponent.h"
#include "Components/MovementComponent.h"
#include "CameraComponent.h"
//

CPlayState::CPlayState(StateStack& aStateStack, const int aLevelIndex, const bool aShouldReturnToLevelSelect)
	: State(aStateStack)
	, myLevelIndex(aLevelIndex)
	, myShouldReturnToLevelSelect(aShouldReturnToLevelSelect)
{
	myIsLoaded = false;
}


CPlayState::~CPlayState()
{
	SAFE_DELETE(myGameObjectManager);
	SAFE_DELETE(myGUIManager);
	//SAFE_DELETE(myControllerComponent);
	CPhysicsManager::DestroyInstance();
	
	LoadManager::DestroyInstance();
	//EnemyFactory::DestroyInstance(); TODO: delete this class
	
	CPhysicsManager::DestroyInstance();
	CModelComponentManager::Destroy();
	CAudioSourceComponentManager::Destroy();
	CParticleEmitterComponentManager::Destroy();
	CCameraManager::Destroy();
	InputControllerManager::DestroyInstance();
	MovementComponentManager::DestroyInstance();

	PollingStation::NullifyLevelSpecificData();

	CComponentManager::DestroyInstance();
}

void CPlayState::Load()
{
	myGameObjectManager = new CGameObjectManager();
	myGUIManager = new GUI::GUIManager();
	//myGUIManager->Init("Models/gui/gui.fbx", true);

	CreateManagersAndFactories();

	MODELCOMP_MGR.SetScene(&myScene);


	CU::TimerManager timerMgr;
	CU::TimerHandle handle = timerMgr.CreateTimer();
	timerMgr.StartTimer(handle);

	CAMERA->Translate({ 0.0f, 0.f, -11.f });

	Lights::SDirectionalLight dirLight;
	dirLight.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	dirLight.direction = { 0.0f, 0.0f, 1.0f, 1.0f };
	myScene.AddDirectionalLight(dirLight);


	LOAD_MGR.SetCurrentScene(&myScene);
	LOAD_MGR.SetCurrentPlayState(this);

	SSlua::ArgumentList levelIndex(1);
	levelIndex.Add(SSArgument(ssLuaNumber(myLevelIndex)));
	//LUA_WRAPPER.CallLuaFunction("GameLoad", levelIndex);

	timerMgr.UpdateTimers();
	//hue hue dags att fula ner play state - Alex(Absolut inte Marcus); // snälla slå Johan inte mig(Alex);
	CGameObject* tempPlayerObject = myGameObjectManager->CreateGameObject();
	CGameObject* tempWorldObject = myGameObjectManager->CreateGameObject();
	CModelComponent* tempModelComponentForWorldObject = CModelComponentManager::GetInstance().CreateComponent("Models/Player/swarmerShip.fbx");
	tempWorldObject->GetLocalTransform().Move(CU::Vector3f(0.0f, 000.0f, 500.0f));
	tempWorldObject->AddComponent(tempModelComponentForWorldObject);
	CGameObject* tempWorldObject2 = myGameObjectManager->CreateGameObject();
	CModelComponent* tempModelComponentForWorldObject2 = CModelComponentManager::GetInstance().CreateComponent("Models/Player/swarmerShip.fbx");
	tempWorldObject2->GetLocalTransform().Move(CU::Vector3f(500.0f, 0.0f, 0.0f));
	tempWorldObject2->AddComponent(tempModelComponentForWorldObject2);
	myCameraObject = globalGame->GetObjectManagerReference().CreateGameObject();
	InputController* tempInputController = new InputController();
	InputControllerManager::GetInstance().RegisterComponent(tempInputController);
	tempPlayerObject->AddComponent(tempInputController);
	MovementComponent* tempMovementController = new MovementComponent();
	MovementComponentManager::GetInstance().RegisterComponent(tempMovementController);
	tempPlayerObject->AddComponent(new NavigationComponent());
	tempPlayerObject->AddComponent(tempMovementController);
	CModelComponent* tempModelComponent = CModelComponentManager::GetInstance().CreateComponent("Models/Player/swarmerShip.fbx");
	//myScene.AddModelInstance(tempModelComponent->GetModelInst());
	tempPlayerObject->AddComponent(tempModelComponent);
	myCameraObject->AddComponent(CCameraManager::GetInstance().CreateCameraComponent());
	myCameraObject->GetLocalTransform().SetPosition(CU::Vector3f(0.0f, 0.0f, -1000.0f));
	tempPlayerObject->GetLocalTransform().SetPosition(CU::Vector3f(0.0f, 0.0f, 0.0f));
	CU::Matrix33f camerarotationMatrix = myCameraObject->GetLocalTransform().GetRotation();
	camerarotationMatrix.LookAt(myCameraObject->GetWorlPosition(), tempPlayerObject->GetWorlPosition());
	myCameraObject->GetLocalTransform().SetRotation(camerarotationMatrix);
	tempPlayerObject->AddComponent(myCameraObject);
	CU::Matrix44f cameraTransformation = CAMERA->GetTransformation();
	cameraTransformation.Rotate(0.2f, CU::Axees::X);
	cameraTransformation.Move(CU::Vector3f(0.0f, -100000.0f, 0.0f));
	myCameraObject->GetLocalTransform() = cameraTransformation;
	CAMERA->SetTransformation(cameraTransformation);
	//tempPlayerObject->GetLocalTransform() = CAMERA->GetTransformation();
	tempPlayerObject->GetLocalTransform().Move(CU::Vector3f(0.0f, 0.0f, 10));
	tempPlayerObject->NotifyComponents(eComponentMessageType::eMoving, SComponentMessageData());
	//
	SShape shape;
	shape.shape = eModelShape::eSphere;
	
	float time = timerMgr.GetTimer(handle).GetLifeTime().GetMilliseconds();
	myIsLoaded = true;

	Audio::CAudioInterface::GetInstance()->PostEvent("PlayerMoving_Stop");

	GAMEPLAY_LOG("Game Inited in %f ms", time);
}

void CPlayState::Init()
{
	//Load();
}

State::eStatus CPlayState::Update(const CU::Time& aDeltaTime)
{
	//ENGINE->SetCamera(&CCameraManager::GetInstance().GetActiveCamera());
	CU::Matrix44f cameraTransformation = CAMERA->GetTransformation();
	CU::Matrix44f newRotation;
	newRotation.SetPosition(CU::Vector3f(0.0f, -1000.0f, 0.0f));
	newRotation.Rotate(PI / -2.0f, CU::Axees::X);
	cameraTransformation.SetRotation(newRotation);
	cameraTransformation.SetPosition(CU::Vector3f(0.0f, -1000.0f, 0.0f));
	CAMERA->SetTransformation(cameraTransformation);
	//CCameraManager::GetInstance().GetActiveCamera().SetTransformation(cameraTransformation);
	myCameraObject->GetLocalTransform() = cameraTransformation;
	CAMERA->SetTransformation(CCameraManager::GetInstance().GetActiveCamera().GetTransformation());
	myScene.SetCamera(CAMERA);
	//myGUIManager->Update(aDeltaTime)

	Audio::CAudioInterface* audio = Audio::CAudioInterface::GetInstance();
	if (audio != nullptr)
		CAudioSourceComponentManager::GetInstance().Update();

	CPhysicsManager::GetInstance().Update();

	
	CParticleEmitterComponentManager::GetInstance().UpdateEmitters(aDeltaTime);
	InputControllerManager::GetInstance().Update(aDeltaTime);
	MovementComponentManager::GetInstance().Update(aDeltaTime);

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
		int levelIndex = (myLevelIndex > 0) ? myLevelIndex + 1 : 2;
		PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, ChangeLevel(levelIndex)));
	}
}

eMessageReturn CPlayState::Recieve(const Message & aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}

void CPlayState::CreateManagersAndFactories()
{
	CComponentManager::CreateInstance();
	CPhysicsManager::CreateInstance();
	LoadManager::CreateInstance();
	CAudioSourceComponentManager::Create();
	CModelComponentManager::Create();
	CParticleEmitterComponentManager::Create();
	CParticleEmitterComponentManager::GetInstance().SetScene(&myScene);
	CCameraManager::Create();
	InputControllerManager::CreateInstance();
	MovementComponentManager::CreateInstance();
}
