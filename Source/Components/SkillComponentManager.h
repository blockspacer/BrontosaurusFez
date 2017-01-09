#pragma once
class SkillComponent;
class SkillComponentManager
{
public:
	static SkillComponentManager& GetInstance();
	static void CreateInstance();
	static void DestroyInstance();

	SkillComponent* CreateAndRegisterComponent();
	void Update(const CU::Time& aDeltaTime);
private:
	SkillComponentManager();
	~SkillComponentManager();

	CU::GrowingArray<SkillComponent*> myComponents;
	static SkillComponentManager* ourInstance;
};

