#pragma once
#include "Component.h"

class CScriptComponent : public CComponent
{
public:
	enum class eInitSuccess
	{
		eOK,
		eNotRegisteredComponent,
		eInvalidPath,
		eFailedToReadFile,
		eBadLuaCode,
		eNoFunctionTable
	};

	union ScriptErrorData
	{
		std::string* myString;

	};

	CScriptComponent();
	~CScriptComponent();

	eInitSuccess Init(const std::string& aScriptPath, const int aFunctionTableIndex);
	void Call();
	
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;

	static bool HandleError(const eInitSuccess aErrorCode);

private:
	void Destroy() override;

	CU::GrowingArray<eComponentMessageType> mySubscribedComponentMessages;
	int myFunctionTableIndex;

	static std::string ourLastErrorMessage;
};

