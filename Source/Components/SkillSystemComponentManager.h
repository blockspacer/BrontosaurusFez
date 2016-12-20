#pragma once
class SkillSystemComponent;
class SkillSystemComponentManager
{
public:

	static SkillSystemComponentManager& GetInstance();
	static void CreateInstance();
	static void DestroyInstance();

	void RegisterComponent(SkillSystemComponent* aComponent);
	void Update(const CU::Time& aDeltaTime);
	~SkillSystemComponentManager();
private:
	SkillSystemComponentManager();

	CU::GrowingArray<SkillSystemComponent*> myComponents;

	static SkillSystemComponentManager* ourInstance;
};

