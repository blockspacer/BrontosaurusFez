#pragma once
class SkillComponent;
struct SkillData;
class CGameObject;
class SkillComponentManager
{
public:
	static SkillComponentManager& GetInstance();
	static void CreateInstance();
	static void DestroyInstance();

	SkillComponent* CreateAndRegisterComponent(CGameObject* aGameObject, SkillData* someData);
	void Update(const CU::Time& aDeltaTime);
private:
	SkillComponentManager();
	~SkillComponentManager();

	CU::GrowingArray<SkillComponent*> myComponents;
	static SkillComponentManager* ourInstance;
};

