#include "stdafx.h"
#include "LoadAIController.h"
#include "KevinLoader/KevinLoader.h"
#include "LoadManager.h"
#include "ComponentManager.h"
#include "AIControllerManager.h"
#include "AIControllerComponent.h"
#include "FleeController.h"
#include "SeekController.h"
#include "WanderController.h"
#include "InputControllerManager.h"
#include "FleeControllerManager.h"
#include "SeekControllerManager.h"

int LoadWanderController(KLoader::SLoadedComponentData someData)
{
	//CWanderController* wander = new CWanderController();
	//CComponentManager::GetInstance().RegisterComponent(wander);
	return 0;
}

int LoadSeekController(KLoader::SLoadedComponentData someData)
{
	CSeekController* seek = CSeekControllerManager::GetInstance().CreateAndRegister();
	
	//set stuff

	seek->SetAggroRange(someData.myData.at("AggroRange").GetFloat());
	seek->SetMaxAcceleration(someData.myData.at("MaxAcceleration").GetFloat());
	seek->SetMaxSpeed(someData.myData.at("MaxSpeed").GetFloat());
	seek->SetSlowDownRadius(someData.myData.at("SlowDownRadius").GetFloat());
	seek->SetWeight(someData.myData.at("Weight").GetFloat());

	return seek->GetId();
}

int LoadFleeController(KLoader::SLoadedComponentData someData)
{
	CFleeController* flee = FleeControllerManager::GetInstance().CreateAndRegisterController();
	
	//set stuff
	flee->SetFleeRadius(someData.myData.at("FleeRadius").GetFloat());
	flee->SetWeight(someData.myData.at("Weight").GetFloat());

	return flee->GetId();
}

int LoadAIController(KLoader::SLoadedComponentData someData)
{
	CAIControllerComponent* AIContainer = new CAIControllerComponent();
	CComponentManager::GetInstance().RegisterComponent(AIContainer);

	AIControllerManager::GetInstance().AddController(AIContainer);

	return AIContainer->GetId();
}
