#pragma once
class DropComponent;
class DropComponentManager
{
public:
	static DropComponentManager& GetInstance();
	static void CreateInstance();
	static void DestroyInstance();

	DropComponent* CreateAndRegisterComponent(unsigned short aAmountToDrop);
	void Update(const CU::Time& aDeltaTime);
private:
	DropComponentManager();
	~DropComponentManager();


	CU::GrowingArray<DropComponent*> myComponents;
	static DropComponentManager* ourInstance;
};

