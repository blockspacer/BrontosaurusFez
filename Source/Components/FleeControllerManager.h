#pragma once

class CFleeController;

class FleeControllerManager
{
public:
	static void Create();
	static FleeControllerManager& GetInstance();
	static void Destroy();

	CFleeController* CreateAndRegisterController();


private:
	FleeControllerManager();
	~FleeControllerManager();



private:
	CU::GrowingArray<CFleeController*> myControllers;

	static FleeControllerManager* ourInstance;
};

