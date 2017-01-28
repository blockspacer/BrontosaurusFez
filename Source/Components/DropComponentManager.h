#pragma once
class DropComponent;
class DropComponentManager
{
public:
	static DropComponentManager& GetInstance();
	static void CreateInstance();
	static void DestroyInstance();

	DropComponent* CreateAndRegisterComponent(unsigned short aAmountToDrop, unsigned short aHealthGlobePercentChance, unsigned short aManaGlobePercentChance, const char* aHatDrop);
	void Update(const CU::Time& aDeltaTime);
private:
	DropComponentManager();
	~DropComponentManager();


	CU::GrowingArray<DropComponent*> myComponents;
	static DropComponentManager* ourInstance;
};

