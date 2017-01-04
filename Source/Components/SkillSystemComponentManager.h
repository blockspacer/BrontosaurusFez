#pragma once
class SkillSystemComponent;
class CGameObjectManager;
class CCollisionComponentManager;
class SkillSystemComponentManager
{
public:

	static SkillSystemComponentManager& GetInstance();
	static void CreateInstance();
	static void DestroyInstance();

	void RegisterComponent(SkillSystemComponent* aComponent);
	void Update(const CU::Time& aDeltaTime);
	void SetGameObjectManager(CGameObjectManager* aGameObjectMan);
	void SetCollisionComponentManager(CCollisionComponentManager* aCollisionComponentManager);
	CGameObjectManager* GetGameObjectManager(); // but why..;
	CCollisionComponentManager* GetCollisionComponentManager();
	~SkillSystemComponentManager();
private:
	SkillSystemComponentManager();

	CU::GrowingArray<SkillSystemComponent*> myComponents;
	CGameObjectManager* myGameObjectManager; // O.o so lazy...
	CCollisionComponentManager* myCollisionComponentManager;

	static SkillSystemComponentManager* ourInstance;
};

