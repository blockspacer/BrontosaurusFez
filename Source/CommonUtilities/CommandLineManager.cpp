#include "stdafx.h"
#include "CommandLineManager.h"

CommandLineManager* CommandLineManager::myInstance = nullptr;

void CommandLineManager::SetArguments(int argc, char * argv[])
{
	if (argc == 0)
	{
		myArgs.Init(1);
	}
	else
	{
		myArgs.Init(argc);
	}
	for (int i = 1; i < argc; ++i)
	{
		myArgs.Add(argv[i]);
	}

	ActivateLogs();
}

void CommandLineManager::ActivateLogs()
{
	if (HasArgument("-activatelog", "gameplay"))
	{
		DL_Debug::Debug::GetInstance()->Activate(DL_Debug::eLogTypes::eGamePlay);
	}
	if (HasArgument("-activatelog", "resource"))
	{
		DL_Debug::Debug::GetInstance()->Activate(DL_Debug::eLogTypes::eResource);
	}
	if (HasArgument("-activatelog", "engine"))
	{
		DL_Debug::Debug::GetInstance()->Activate(DL_Debug::eLogTypes::eEngine);
	}
	if (HasArgument("-activatelog", "crash"))
	{
		DL_Debug::Debug::GetInstance()->Activate(DL_Debug::eLogTypes::eCrash);
	}
	if (HasArgument("-activatelog", "threadedModels"))
	{
		DL_Debug::Debug::GetInstance()->Activate(DL_Debug::eLogTypes::eThreadedModels);
	}
}

CommandLineManager* CommandLineManager::GetInstance()
{
	if (myInstance == nullptr)
	{
		myInstance = new CommandLineManager();
	}
	return myInstance;
}

void CommandLineManager::DestroyInstance()
{
	if (myInstance != nullptr)
	{
		SAFE_DELETE(myInstance);
	}
}

bool CommandLineManager::HasParameter(const char* aParameter)
{
	for (unsigned int i = 0; i < myArgs.Size(); ++i)
	{
		if (myArgs[i] == aParameter)
		{
			return true;
		}
	}
	return false;
}
std::string CommandLineManager::GetParameter(const char* aParameter)
{
	for (unsigned int i = 0; i < myArgs.Size(); ++i)
	{
		if (myArgs[i] == aParameter)
		{
			return myArgs[i];
		}
	}
	return false;
}

std::string CommandLineManager::GetArgument(const char * aParameter)
{
	for (unsigned int i = 0; i < myArgs.Size(); ++i)
	{
		if (myArgs[i] == aParameter)
		{
			for (unsigned int j = i + 1; j < myArgs.Size(); ++j)
			{
				return myArgs[j];
			}
		}
	}
	return false;
}

bool CommandLineManager::HasArgument(const char* aParameter, const char* aArgument)
{
	for (unsigned int i = 0; i < myArgs.Size(); ++i)
	{
		if (myArgs[i] == aParameter)
		{
			for (unsigned int j = i + 1; j < myArgs.Size(); ++j)
			{
				if (myArgs[j] == aArgument)
				{
					return true;
				}
				else if (myArgs[j][0] == '-')
				{
					return false;
				}
			}
		}
	}
	return false;
}

CommandLineManager::CommandLineManager()
{
}


CommandLineManager::~CommandLineManager()
{
}
