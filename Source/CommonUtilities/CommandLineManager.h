#pragma once
#include <string>
#include "GrowingArray.h"
class CommandLineManager
{
public:
	void SetArguments(int argc, char* argv[]);
	void ActivateLogs();
	static CommandLineManager* GetInstance();
	static void DestroyInstance();
	bool HasParameter(const char* aParameter);
	bool HasArgument(const char* aParameter, const char* aArgument);
	std::string GetParameter(const char* aParameter);
	std::string GetArgument(const char* aParameter);
private:
	CommandLineManager();
	~CommandLineManager();
	static CommandLineManager* myInstance;
	
	CU::GrowingArray<std::string> myArgs;
};

