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
#include "Components\BlessingTowerComponentManager.h"
#include "ChangeLevelTriggerComponentManager.h"
#include "MainStatComponent.h"
#include "Components/HealthRestoreTriggerComponentManager.h"
#include "Components/PointLightComponentManager.h"
#include "Components/PointLightComponent.h"

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
#include "PlayerHealthMessenger.h"
#include "PlayerManaMessenger.h"

//ULTRA TEMP INCLUDES, remove if you see and remove the things that don't compile afterwards
#include "../Components/ScriptComponentManager.h"
#include "ParticleEffectManager.h"
#include "PostMaster/SecretlySetMousePos.h"

CPlayState::CPlayState(StateStack& aStateStack, const int aLevelIndex, const bool aShouldReturnToLevelSelect)
	: State(aStateStack)
	, myLevelIndex(aLevelIndex)
	, myShouldReturnToLevelSelect(aShouldReturnToLevelSelect)
	, myScene(nullptr)
	, myMouseComponent(nullptr)
	, myQuestManager()
	, myQuestDrawer(myQuestManager)
	, myShuldRenderNavmesh(false)
	, myCameraIsFree(false)
	, myCameraKeysDown(false)
{
	myIsLoaded = false;
	PostMaster::GetInstance().Subscribe(this, eMessageType::eHatAdded);
	PostMaster::GetInstance().Subscribe(this, eMessageType::eGoldChanged);
	

}

CPlayState::~CPlayState()
{

	Audio::CAudioInterface* audioInterface = Audio::CAudioInterface::GetInstance();
	if (audioInterface != nullptr)
	{
		audioInterface->PostEvent("switchBank");

		//audioInterface->PostEvent("StopBayBlade");
		CU::CJsonValue levelsFile;

		std::string errorString = levelsFile.Parse("Json/LevelList.json");
		if (!errorString.empty()) DL_MESSAGE_BOX(errorString.c_str());

		CU::CJsonValue levelsArray = levelsFile.at("levels");

		std::string temp = "Stop";
		temp += levelsArray[myLevelIndex].GetString();

		Audio::CAudioInterface::GetInstance()->PostEvent(levelsArray[myLevelIndex].GetString().c_str());


		audioInterface->UnLoadBank("Audio/playState.bnk");
	}


	//Don forgetti to deletti
	SAFE_DELETE(myEmitterComp);
	SAFE_DELETE(myCollisionComponentManager);
	SAFE_DELETE(myScriptComponentManager);
	SAFE_DELETE(myParticleEffectManager);
	//SAFE_DELETE(myStatManager);
	SAFE_DELETE(myGoldText);
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
	CPickupFactory::Destroy();
	CPickupManager::DestroyInstance();
	RespawnComponentManager::Destroy();
	CMasterAI::Destroy();
	BlessingTowerComponentManager::DestroyInstance();
	CEnemyFactory::Destroy();
	SkillFactory::DestroyInstance();
	CHealthRestoreTriggerComponentManager::Destroy();
	CPointLightComponentManager::Destroy();
	CChangeLevelTriggerComponentManager::Destroy();
	CComponentManager::DestroyInstance();
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eHatAdded);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eGoldChanged);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eKeyboardMessage);

	CLevelManager::DestroyInstance();
}

void CPlayState::Load()
{
	//start taking the time for loading level
	if (myShouldReturnToLevelSelect == true)
	{
		myLevelIndex++;
	}
	CU::TimerManager timerMgr;
	CU::TimerHandle handle = timerMgr.CreateTimer();
	timerMgr.StartTimer(handle);
	srand(static_cast<unsigned int>(time(NULL)));

	CreateManagersAndFactories();
	LoadManagerGuard loadManagerGuard;

	CShopStorage::GetInstance().LoadStorage("Json/Hats/HatBluePrints.json");

	MODELCOMP_MGR.SetScene(myScene);
	LoadManager::GetInstance().SetCurrentPlayState(this);
	LoadManager::GetInstance().SetCurrentScene(myScene);


	Lights::SDirectionalLight dirLight;
	dirLight.color = { .35f, .35f, .35f, 1.0f };
	dirLight.direction = { -1.0f, -1.0f, 1.0f, 1.0f };
	myScene->AddDirectionalLight(dirLight);

	CONSOLE->GetLuaFunctions();

	myScene->AddCamera(CScene::eCameraType::ePlayerOneCamera);
	CU::Camera& playerCamera = myScene->GetCamera(CScene::eCameraType::ePlayerOneCamera);
	playerCamera.Init(50, WINDOW_SIZE_F.x, WINDOW_SIZE_F.y, 1.f, 75000.0f);

	SkillFactory::GetInstance().RegisterSkills();

	CCameraComponent* cameraComponent = CCameraComponentManager::GetInstance().CreateCameraComponent();
	cameraComponent->SetCamera(myScene->GetCamera(CScene::eCameraType::ePlayerOneCamera));

	myChangeTexts.Init(5);

	myGoldText = new CTextInstance;
	myGoldText->SetColor({ 0.f, 0.f , 0.f, 1.f });
	myGoldText->SetPosition(CU::Vector2f(0.575f, 0.95f));
	myGoldText->SetText("");
	myGoldText->Init();

	//Loading
	CU::CJsonValue levelsFile;

	std::string errorString = levelsFile.Parse("Json/LevelList.json");
	if (!errorString.empty()) DL_MESSAGE_BOX(errorString.c_str());

	CU::CJsonValue levelsArray = levelsFile.at("levels");

#ifdef _DEBUGq
	myLevelIndex = levelsArray.Size() - 1;
#else
	const int levelIndex = 0;
#endif

	std::string levelPath = "Json/Levels/";
	levelPath += levelsArray[myLevelIndex].GetString();
	levelPath += "/LevelData.json";

	std::string questPath = "Json/Quests/";
	questPath += levelsArray[myLevelIndex].GetString();
	questPath += ".json";

	std::string navmeshPath = "Models/Navmesh/";
	navmeshPath += levelsArray[myLevelIndex].GetString();
	navmeshPath -= ".json";
	navmeshPath += "_navmesh.obj";

	//NAVMESH
	{
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
	}

	// Cubemap & Skybox
	std::string cubemapPath = "Models/Cubemaps/";
	cubemapPath += levelsArray[myLevelIndex].GetString();
	cubemapPath -= ".json";
	cubemapPath += "_cubemap.dds";
	{
		std::ifstream infile(cubemapPath);
		if (infile.good())
		{
			myScene->SetSkybox(cubemapPath.c_str());
		}
		else
		{
			myScene->SetSkybox("default_cubemap.dds");
		}
	}

	myQuestManager.LoadQuestlines(questPath);

	KLoader::CKevinLoader &loader = KLoader::CKevinLoader::GetInstance();

	const KLoader::eError loadError = loader.LoadFile(levelPath);
	if (loadError != KLoader::eError::NO_LOADER_ERROR)
	{
		DL_ASSERT("Loading Failed");
	}

	CEnemyFactory::GetInstance().Init(levelsArray[myLevelIndex].GetString());
	CPickupFactory::GetInstance().Init(levelsArray[myLevelIndex].GetString());



	if (PollingStation::PlayerInput != nullptr)
	{
		PollingStation::playerObject = PollingStation::PlayerInput->GetParent();

		//Dísclaimer: fult men funkar //lägg till allt spelar specifikt som inte LD behöver störas av här
		CPlayerHealthMessenger* healthMessenger = new CPlayerHealthMessenger();

		//RespawnComponent* respawn = RespawnComponentManager::GetInstance().CreateAndRegisterComponent();

		CComponentManager::GetInstance().RegisterComponent(healthMessenger);

		PollingStation::playerObject->AddComponent(healthMessenger);
		//PollingStation::playerObject->AddComponent(respawn);
		PollingStation::playerObject->AddComponent(new CPlayerHealthMessenger());
		PollingStation::playerObject->AddComponent(new CPlayerManaMessenger());
		PollingStation::playerObject->AddComponent(CPickupManager::GetInstance().CreatePickerUpperComp());
		PollingStation::playerObject->AddComponent(CAudioSourceComponentManager::GetInstance().CreateComponent());
		PollingStation::playerObject->AddComponent(new CMainStatComponent());

		PointLightComponent* pl = CPointLightComponentManager::GetInstance().CreateAndRegisterComponent();

		pl->SetColor({ 1.0f, 1.0f, 1.0f });
		pl->SetIntensity(0.85f);
		pl->SetRange(500.f);
		pl->SetOffsetToParent(CU::Vector3f(0.f, 100.f, 0.f));

		PollingStation::playerObject->AddComponent(pl);


		////TEMP CARL BEGIN

		//CScriptComponent* scriptComp = myScriptComponentManager->CreateComponent("Script/exempel_script.lua");
		//PollingStation::playerObject->AddComponent(scriptComp);

		////TEMP CARL END
	}



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

	myGameObjectManager->SendObjectsDoneMessage();


	for (unsigned int i = 0; i < PollingStation::myThingsEnemiesShouldAvoid.Size(); i++)
	{
		PollingStation::myThingsEnemiesShouldAvoid[i]->AddComponent(CAudioSourceComponentManager::GetInstance().CreateComponent());
	}

	//Give hats on level entry
	myHatMaker->LoadBluePrints("Json/Hats/HatBluePrints.json");
	if (myShouldReturnToLevelSelect == false)
	{
		myHatMaker->GiveTheManAHat();
	}
	else
	{
		std::string loadHatsMajiggerPath = "Json/Hats/LevelSelectHatData.json";
		CU::CJsonValue HatBluePrint;
		const std::string& errorString = HatBluePrint.Parse(loadHatsMajiggerPath);
		CU::CJsonValue loadHatsMajigger = HatBluePrint.at("HatsOnlevelEntryList");
		std::string levelname = levelsArray[myLevelIndex].GetString();
		for (unsigned int i = 0; i < loadHatsMajigger.Size(); ++i)
		{
			if (loadHatsMajigger[i].at("LevelName").GetString() == levelname)
			{
				CU::CJsonValue hatArray = loadHatsMajigger[i].at("HatArray");
				for (unsigned int j = 0; j < hatArray.Size(); ++j)
				{
					myHatMaker->MakeHatFromBluePrint(hatArray[j].GetString());
				}
			}
		}
	}

	CSecretlySetMousePos::SetCamera(myScene->GetCamera(CScene::eCameraType::ePlayerOneCamera));

	myIsLoaded = true;

	//get time to load the level:
	timerMgr.UpdateTimers();
	float time = timerMgr.GetTimer(handle).GetLifeTime().GetMilliseconds();
	GAMEPLAY_LOG("Game Inited in %f ms", time);
}

void CPlayState::Init()
{
	//skillnad på load, init & konstructor ?
	//	svar:	1. konstruktor ska sätta allt till noll/null
	//			2. init ska skapa allt som kan bero på ifall motorn är skapad etc
	//			3. load kallas trådat för init kallas alltid av statestacken när den pushar ett nytt state


	//start sound


	Audio::CAudioInterface::GetInstance()->LoadBank("Audio/playState.bnk");

	CU::CJsonValue levelsFile;

	std::string errorString = levelsFile.Parse("Json/LevelList.json");
	if (!errorString.empty()) DL_MESSAGE_BOX(errorString.c_str());

	CU::CJsonValue levelsArray = levelsFile.at("levels");

	Audio::CAudioInterface::GetInstance()->PostEvent(levelsArray[myLevelIndex].GetString().c_str());

	//myGameEventMessenger.Init({ 0.5f, 0.1f });
}

eStateStatus CPlayState::Update(const CU::Time& aDeltaTime)
{
	Audio::CAudioInterface* audio = Audio::CAudioInterface::GetInstance();
	if (audio != nullptr)
	{
		CAudioSourceComponentManager::GetInstance().Update();
		if (PollingStation::playerData->myIsWhirlwinding == true)
		{
			audio->PostEvent("WhirlWind");
		}
		else
		{
			audio->PostEvent("StopWhirlWind");
		}
	}

	myHatMaker->Update();


	CParticleEmitterComponentManager::GetInstance().UpdateEmitters(aDeltaTime);
	CPointLightComponentManager::GetInstance().Update(aDeltaTime);
	InputControllerManager::GetInstance().Update(aDeltaTime);

	if (myCameraIsFree == true)
	{
		UpdateCamera(aDeltaTime.GetSeconds());
	}
	
	MovementComponentManager::GetInstance().Update(aDeltaTime);
	AIControllerManager::GetInstance().Update(aDeltaTime);
	SkillSystemComponentManager::GetInstance().Update(aDeltaTime);
	CPickupManager::GetInstance().Update(aDeltaTime);
	RespawnComponentManager::GetInstance().Update(aDeltaTime);
	myMouseComponent->Update(aDeltaTime);
	if (myGUIManager)
	{
		myGUIManager->Update(aDeltaTime);
	}

	myCollisionComponentManager->Update();
	myParticleEffectManager->Update();
	myScene->Update(aDeltaTime);

	myGameObjectManager->DestroyObjectsWaitingForDestruction();
	std::string goldAmount = "Gold: ";
	goldAmount += std::to_string(PollingStation::playerData->GetGold());
	myGoldText->SetText(goldAmount.c_str());

	SkillComponentManager::GetInstance().Update(aDeltaTime);
	DropComponentManager::GetInstance().Update(aDeltaTime);

	myHealthBarManager->Update();
	BlessingTowerComponentManager::GetInstance().Update(aDeltaTime);
	CChangeLevelTriggerComponentManager::GetInstance().Update();

	for (unsigned int i = 0; i < myChangeTexts.Size(); ++i)
	{
		CU::Vector2f position = myChangeTexts[i]->GetPosition();

		position.y += 1 * aDeltaTime.GetSeconds();
		myChangeTexts[i]->SetPosition(position);
	}
	//myGameEventMessenger.Update(aDeltaTime.GetSeconds());

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

	msg.myBlendState = eBlendState::eAlphaBlend;
	msg.myDepthStencilState = eDepthStencilState::eDisableDepth;
	msg.myRasterizerState = eRasterizerState::eNoCulling;
	msg.mySamplerState = eSamplerState::eClamp;
	RENDERER.AddRenderMessage(new SChangeStatesMessage(msg));
	myGoldText->Render();

	for (unsigned int i = 0; i < myChangeTexts.Size(); ++i)
	{
		myChangeTexts[i]->Render();
	}

	myQuestDrawer.Render();
	//myGameEventMessenger.Render();
}

void CPlayState::OnEnter(const bool aLetThroughRender)
{
	PostMaster::GetInstance().Subscribe(this, eMessageType::eKeyboardMessage);


	//Audio::CAudioInterface::GetInstance()->PostEvent("BayBlade");
	if (!aLetThroughRender)
	{
		myGUIManager->RestartRenderAndUpdate();
	}

	myQuestManager.CompleteEvent();
}

void CPlayState::OnExit(const bool aLetThroughRender)
{
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eKeyboardMessage);

	if (!aLetThroughRender)
	{
		myGUIManager->PauseRenderAndUpdate();
	}
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

void CPlayState::ChangeGoldAmount(const int aValue, const bool aDecreaseGold)
{
	PollingStation::playerData->AddGold(aValue);

	myChangeTexts.Add(new CTextInstance());
	CTextInstance* text = myChangeTexts.GetLast();

	text->SetColor(CTextInstance::Black);
	text->SetPosition(myGoldText->GetPosition());
	if (aDecreaseGold == false)
	{
		text->SetText("+" + aValue);
	}
	if (aDecreaseGold == true)
	{
		text->SetText("-" + aValue);
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

void CPlayState::CameraMovement(const CU::eKeys& aKey, bool pressed)
{
	switch (aKey)
	{
	case CU::eKeys::W:
		myCameraKeysDown[0] = pressed;
		break;
	case CU::eKeys::S:
		myCameraKeysDown[1] = pressed;
		break;
	case CU::eKeys::A:
		myCameraKeysDown[2] = pressed;
		break;
	case CU::eKeys::D:
		myCameraKeysDown[3] = pressed;
		break;
	case CU::eKeys::Q:
		myCameraKeysDown[4] = pressed;
		break;
	case CU::eKeys::E:
		myCameraKeysDown[5] = pressed;
		break;
	case CU::eKeys::UP:
		myCameraKeysDown[6] = pressed;
		break;
	case CU::eKeys::DOWN:
		myCameraKeysDown[7] = pressed;
		break;
	case CU::eKeys::LEFT:
		myCameraKeysDown[8] = pressed;
		break;
	case CU::eKeys::RIGHT:
		myCameraKeysDown[9] = pressed;
		break;
	default:
		break;
	}



}

void CPlayState::UpdateCamera(const float dt)
{
	CU::Camera& camera = myScene->GetCamera(CScene::eCameraType::ePlayerOneCamera);
	float deltaTime = dt;
	float speed = 500.f;
	float rotspeed = 50.f;
	rotspeed = DEGREES_TO_RADIANS(rotspeed);

	if(myCameraKeysDown[0] == true)
		camera.TranslateForward(speed * deltaTime);
	else if (myCameraKeysDown[1] == true)
		camera.TranslateForward(-speed * deltaTime);
	
	if (myCameraKeysDown[2] == true)
		camera.TranslateSideways(-speed * deltaTime);
	else if (myCameraKeysDown[3] == true)
		camera.TranslateSideways(speed * deltaTime);
	
	if (myCameraKeysDown[4] == true)
		camera.Roll(rotspeed * deltaTime);
	else if (myCameraKeysDown[5] == true)
		camera.Roll(-rotspeed * deltaTime);
	
	if (myCameraKeysDown[6] == true)

		camera.Pitch(rotspeed * deltaTime);
	else if (myCameraKeysDown[7] == true)
		camera.Pitch(-rotspeed * deltaTime);
	
	
	if (myCameraKeysDown[8] == true)
		camera.Jaw(-rotspeed * deltaTime);
	else if (myCameraKeysDown[9] == true)
		camera.Jaw(rotspeed * deltaTime);
}

void CPlayState::FlipCameraUnlocked()
{
	myCameraIsFree = !myCameraIsFree;
	SComponentMessageData data;
	data.myBool = myCameraIsFree;
	PollingStation::playerObject->NotifyComponents(eComponentMessageType::eUnlockCamera, data);
}

void CPlayState::CreateManagersAndFactories()
{
	myScene = new CScene();
	myGameObjectManager = new CGameObjectManager();
	myGUIManager = new GUI::GUIManager();
	myGUIManager->Init("Models/gui/guiBase.fbx");

	myCollisionComponentManager = new CCollisionComponentManager();
	myScriptComponentManager = new CScriptComponentManager();
	myParticleEffectManager = new CParticleEffectManager(*myScene);
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
	CEnemyFactory::Create(*myGameObjectManager, *myCollisionComponentManager, *myHealthBarManager);
	BlessingTowerComponentManager::CreateInstance();
	CHealthRestoreTriggerComponentManager::Create();
	CPointLightComponentManager::Create(*myScene);
	CChangeLevelTriggerComponentManager::Create();
}
