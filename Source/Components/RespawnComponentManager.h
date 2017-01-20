#pragma once

class RespawnComponent;

namespace CU
{
	class Time;
}

class RespawnComponentManager
{
public:
	static void Create();
	static void Destroy();
	static RespawnComponentManager& GetInstance();

	RespawnComponent* CreateAndRegisterComponent();
	void Update(const CU::Time& aDeltaTime);

private:
	RespawnComponentManager();
	~RespawnComponentManager();


private:
	CU::GrowingArray<RespawnComponent*> myComponents;

	static RespawnComponentManager* ourInstance;
};

