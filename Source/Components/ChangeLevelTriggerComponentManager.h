#pragma once

class ChangeLevelTriggerComponent;

class CChangeLevelTriggerComponentManager
{
public:
	static void Create();
	static void Destroy();
	static CChangeLevelTriggerComponentManager& GetInstance();
	ChangeLevelTriggerComponent* CreateAndRegisterComponent(const unsigned char aLevelToGoTo, const unsigned short aPortalID);

	CChangeLevelTriggerComponentManager();
	~CChangeLevelTriggerComponentManager();

	void Update();

private:
	static CChangeLevelTriggerComponentManager* ourInstance;
	CU::GrowingArray<ChangeLevelTriggerComponent*> myComponents;
};
