#pragma once
class DropComponent;
class DropComponentManager
{
public:
	static DropComponentManager& GetInstance();
	static void CreateInstance();
	static void DestroyInstance();

	DropComponent* CreateAndRegisterComponent(unsigned short aAmountToDrop, unsigned short aPercentChance);
	void Update(const CU::Time& aDeltaTime);
private:
	DropComponentManager();
	~DropComponentManager();


	CU::GrowingArray<DropComponent*> myComponents;
	static DropComponentManager* ourInstance;
};

