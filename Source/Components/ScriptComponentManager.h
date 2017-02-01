#pragma once

class CScriptComponent;

class CScriptComponentManager
{
public:
	CScriptComponentManager();
	~CScriptComponentManager();

	CScriptComponent* CreateComponent(const std::string& aScriptPath/*, const std::string& aInitFunction*/);
	void DestroyComponent(CScriptComponent* aScriptComponent);

	static CScriptComponentManager* GetInstance();

private:
	CU::GrowingArray<CScriptComponent*> myComponents;

	static CScriptComponentManager* ourInstance;
};

