#pragma once

class HealthRestoreTriggerComponent;

class CHealthRestoreTriggerComponentManager
{
public:
	static void Destroy();
	static void Create();
	static CHealthRestoreTriggerComponentManager& GetInstance();

	HealthRestoreTriggerComponent* CreateAndRegisterComponent();

private:
	CHealthRestoreTriggerComponentManager();
	~CHealthRestoreTriggerComponentManager();

private:
	static CHealthRestoreTriggerComponentManager* ourInstance;
	CU::GrowingArray<HealthRestoreTriggerComponent*> myComponents;
};

