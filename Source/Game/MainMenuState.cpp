#include "stdafx.h"
#include "MainMenuState.h"
#include "../PostMaster/PostMaster.h"
#include "../PostMaster/Message.h"
#include "../PostMaster/Event.h"

#include "../GUI/GUIManager/GUIManager.h"
#include "BrontosaurusEngine/Skybox.h"
#include "BrontosaurusEngine/Renderer.h"
#include "BrontosaurusEngine/Engine.h"
#include "../Audio/AudioInterface.h"
#include "BackgroundLoadingManager.h"

MainMenuState::MainMenuState(StateStack& aStateStack)
	: State(aStateStack)
	, myGUIManager(nullptr)
	, mySkyBox(nullptr)
	, myIsGoingToLevelSelect(false)
{
}

MainMenuState::~MainMenuState()
{
	SAFE_DELETE(myGUIManager);
	SAFE_DELETE(mySkyBox);
}

void MainMenuState::Init()
{
	//myGUIManager = new GUI::GUIManager(true);
	//myGUIManager->Init("Models/mainMenu/mainMenu.fbx", true);
	//
	//mySkyBox = new CSkybox();
	//mySkyBox->Init();

	//CBackgroundLoadingManager::GetInstance().CreateStateToLoad(myStateStack, -1);
}

State::eStatus MainMenuState::Update(const CU::Time& /*aDeltaTime*/)
{
	//myGUIManager->Update(aDeltaTime);

	return myStatus;
}

void MainMenuState::Render()
{
	//SChangeStatesMessage statemsg;
	//statemsg.myBlendState = eBlendState::eNoBlend;
	//statemsg.myRasterizerState = eRasterizerState::eNoCullingClockwise;
	//statemsg.myDepthStencilState = eDepthStencilState::eDisableDepth;
	//statemsg.mySamplerState = eSamplerState::eClamp;
	//
	//RENDERER.AddRenderMessage(new SChangeStatesMessage(statemsg));
	//
	//SRenderSkyboxMessage msg;
	//msg.mySkybox = mySkyBox;
	//
	//RENDERER.AddRenderMessage(new SRenderSkyboxMessage(msg));
	//
	//myGUIManager->Render();
}

void MainMenuState::OnEnter()
{
	//bool result;
	//result = Audio::CAudioInterface::GetInstance()->LoadBank("Audio/mainMenu.bnk");
	//Audio::CAudioInterface::GetInstance()->PostEvent("PlayBasSong");
	//if (myGUIManager == nullptr)
	//{
	//	myGUIManager = new GUI::GUIManager(true);
	//	myGUIManager->Init("Models/mainMenu/mainMenu.fbx", true);
	//}
	//
	//if (myIsGoingToLevelSelect == false)
	//{
	//	myGUIManager->RestartRenderAndUpdate();
	//}
	//else
	//{
	//	PostMaster::GetInstance().InsertSubscriber(myGUIManager, eMessageType::eMouseMessage);
	//	PostMaster::GetInstance().InsertSubscriber(myGUIManager, eMessageType::eKeyboardMessage);
	//}
	//
	//myIsGoingToLevelSelect = false;
	//
	//myGUIManager->SetSkipEmissive(false);
	//
	//myGUIManager->SetRenderMouse(true);
}

void MainMenuState::OnExit()
{
	//Audio::CAudioInterface::GetInstance()->PostEvent("StopBasSong");
	//Audio::CAudioInterface::GetInstance()->PostEvent("switchBank");
	//Audio::CAudioInterface::GetInstance()->UnLoadBank("Audio/menMenu.bnk");
	//
	////SAFE_DELETE(myGUIManager);
	//if (myIsGoingToLevelSelect == false)
	//{
	//	myGUIManager->PauseRenderAndUpdate();
	//}
	//else
	//{
	//	PostMaster::GetInstance().UnSubscribe(myGUIManager, eMessageType::eMouseMessage);
	//	PostMaster::GetInstance().UnSubscribe(myGUIManager, eMessageType::eKeyboardMessage);
	//}
	//
	//myGUIManager->SetSkipEmissive(true);
	//
	//myGUIManager->SetRenderMouse(false);
}
