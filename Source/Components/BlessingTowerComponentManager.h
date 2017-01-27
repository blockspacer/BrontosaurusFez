#pragma once
class BlessingTowerComponent;
class BlessingTowerComponentManager
{
public:
	static BlessingTowerComponentManager& GetInstance();
	static void CreateInstance();
	static void DestroyInstance();
	void Update(const CU::Time& aDeltaTime);
	BlessingTowerComponent* CreateAndRegisterComponent();
private:
	BlessingTowerComponentManager();
	~BlessingTowerComponentManager();

	CU::GrowingArray<BlessingTowerComponent*> myComponents;
	static BlessingTowerComponentManager* ourInstance;
};

