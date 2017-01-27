#pragma once

class CScriptComponent;

class CScriptComponentManager
{
public:
	CScriptComponentManager();
	~CScriptComponentManager();

	CScriptComponent* CreateComponent(const std::string& aScriptPath);

	static CScriptComponentManager* GetInstance();

private:
	CU::GrowingArray<CScriptComponent*> myComponents;
	int myFunctionTableIndex;

	static CScriptComponentManager* ourInstance;
};

