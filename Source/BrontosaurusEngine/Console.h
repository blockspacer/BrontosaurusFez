#pragma once
#include <map>
#include "../LuaWrapper/SSlua/SSlua.h"
#include "../CommonUtilities/GrowingArray.h"
#include "../PostMaster/Subscriber.h"

namespace CU
{
	enum class eKeys;
}
class CTextInstance;
class CConsole : public Subscriber
{
public:
	CConsole();
	~CConsole();

	void Init();
	void GetLuaFunctions();
	void Activate();
	void Deactivate();
	eMessageReturn Recieve(const Message & aMessage) override;
	eMessageReturn TakeKeyBoardInputPressed(const CU::eKeys aKey);
private:
	std::map<std::string, SSlua::LuaCallbackFunction> myLuaFunctions;
	CU::GrowingArray<CTextInstance*> myTextLog;
	CTextInstance* myCurrentText;
};

