#include "stdafx.h"
#include <stdlib.h>
#include "DL_Debug.h"
#include <stdio.h>
#include <stdarg.h>
#include <ctime>
#include <iomanip>
#include "DL_Assert.h"
#include <iostream>

#include "StringHelper.h"
#include "CommandLineManager.h"

#pragma warning(disable : 4091)
#include "DL_StackWalker.h"
#pragma warning(default : 4091)

#ifdef __APPLE__
#define VSPRINTF vsprintf
#elif defined _WIN32 || defined _WIN64
#define VSPRINTF vsprintf_s
#else
static_assert(false, "Is this debian?");
#endif

#define SUPRESS_UNUSED_WARNING(variable) variable
const unsigned int MAX_STRING_BUFFER_SIZE = 1024u;

namespace DL_Debug
{
	Debug* Debug::ourInstance = nullptr;

	Debug::Debug()
	{

	}

	Debug::~Debug()
	{
	}

	void Debug::CreateLog(eLogTypes aLogType)
	{
		if (myLogFiles.find(aLogType) == myLogFiles.end())
		{
			myLogFiles[aLogType] = Log(); 
			const char* fileName;
			switch (aLogType)
			{
			case DL_Debug::eLogTypes::eEngine:
				fileName = "LOG_ENGINE.loggo";
				break;
			case DL_Debug::eLogTypes::eGamePlay:
				fileName = "LOG_GAMEPLAY.loggo";
				break;
			case DL_Debug::eLogTypes::eResource:
				fileName = "LOG_RESOURCES.loggo";
				break;
			case DL_Debug::eLogTypes::eCrash:
				fileName = "LOG_CRASH.loggo";
				break;
			case DL_Debug::eLogTypes::eThreadedModels:
				fileName = "LOG_THREADED_MODELS.loggo";
				break;
			default:
				fileName = "LOG_DEFAULT.loggo";
				break;
			}
			myLogFiles[aLogType].CreateLog(fileName);
		}
	}

	bool Debug::Destroy()
	{
		if (ourInstance != nullptr)
		{
			SAFE_DELETE(ourInstance);
			return true;
		}

		return false;
	}

	Debug* Debug::GetInstance()
	{
		if (ourInstance == nullptr)
		{
			ourInstance = new Debug();
		}

		return ourInstance;
	}

	void Debug::Activate(eLogTypes aLogType)
	{
		if (myLogFiles.find(aLogType) == myLogFiles.end())
		{
			CreateLog(aLogType);
		}
		
		myLogFiles[aLogType].Activate();
	}

	void Debug::Deactivate(eLogTypes aLogType)
	{
		myLogFiles[aLogType].Deactivate();
	}

	void Debug::AssertMessage(const char *aFileName, const int aLine, const char* /*aFunctionName*/, const char *aString, ...)
	{
		char buffer[MAX_STRING_BUFFER_SIZE] = {};
		va_list args;

		va_start(args, aString);
		VSPRINTF(buffer, aString, args);
		va_end(args);

#ifdef _WIN32
		StackWalker myStackWalker;
		//myDebugFile << myStackWalker.ShowCallstack() << std::endl << std::endl;
		wchar_t fileNameBuffer[MAX_STRING_BUFFER_SIZE];
		wchar_t bufferBuffer[MAX_STRING_BUFFER_SIZE];
		_wassert(CU::CharToWChar(bufferBuffer, buffer), CU::CharToWChar(fileNameBuffer, aFileName), aLine);
#else
		assert(false && charToWChar(aString) && charToWChar(aFileName) && aLine);
#endif
	}

	void Debug::AssertMessage(const char* aFileName, const int aLine, const char* /*aFunctionName*/, const wchar_t* aString, ...)
	{
		wchar_t buffer[MAX_STRING_BUFFER_SIZE] = {};
		va_list args;
		va_start(args, aString);
		wvsprintf(buffer, aString, args);
		va_end(args);

#ifdef _WIN32
		StackWalker myStackWalker;
		//myDebugFile << myStackWalker.ShowCallstack() << std::endl << std::endl;
		wchar_t fileNameBuffer[MAX_STRING_BUFFER_SIZE];
		_wassert(buffer, CU::CharToWChar(fileNameBuffer, aFileName), aLine);
#else
		assert(false && buffer && charToWChar(aFileName) && aLine);
#endif
	}

	void Debug::PrintMessage(const char *aFormattedString, ...)
	{
		char buffer[MAX_STRING_BUFFER_SIZE] = {};
		va_list args;

		va_start(args, aFormattedString);
		VSPRINTF(buffer, aFormattedString, args);
		va_end(args);

		std::cout << buffer << std::endl;
	}

	void Debug::PrintMessage(const wchar_t * aWString, ...)
	{
		wchar_t buffer[MAX_STRING_BUFFER_SIZE] = {};
		va_list args;
		va_start(args, aWString);
		wvsprintf(buffer, aWString, args);
		va_end(args);

		std::wcout << buffer << std::endl;
	}

	void Debug::WriteLog(eLogTypes aLogType, const char* aFormattedString, ...)
	{
		char buffer[MAX_STRING_BUFFER_SIZE] = {};
		va_list args;

		va_start(args, aFormattedString);
		VSPRINTF(buffer, aFormattedString, args);
		va_end(args);

		myLogFiles[aLogType].Write(buffer);
	}

	void Debug::ShowMessageBox(const char* aMessage, ...)
	{
#ifdef _WIN32
		char buffer[MAX_STRING_BUFFER_SIZE] = {};
		va_list args;

		va_start(args, aMessage);
		VSPRINTF(buffer, aMessage, args);
		va_end(args);

		wchar_t wBuffer[MAX_STRING_BUFFER_SIZE] = {};
		CU::CharToWChar(wBuffer, buffer);

		int returnValue = MessageBox(nullptr, wBuffer, L"Friendly error, press cancel to exit, retry if you think you can continue :)", MB_RETRYCANCEL);

		if (returnValue == IDCANCEL)
		{
			DL_ASSERT(wBuffer); // TODO: FIX MINIDUMP AND SHIT
		}
#else
		aMessage;
		//implement mbx for unix
#endif
	}

	void Debug::ShowMessageBox(const wchar_t* aMessage, ...)
	{
#ifdef _WIN32
		wchar_t buffer[MAX_STRING_BUFFER_SIZE] = {};
		va_list args;

		va_start(args, aMessage);
		wvsprintf(buffer, aMessage, args);
		va_end(args);

		MessageBox(GetFocus(), buffer, L"MessageBox :)", MB_HELP);
#else
		aMessage;
		//implement mbx for unix
#endif
	}
	
//	wchar_t* Debug::charToWChar(const char* text)
//	{
//		size_t size = strlen(text) + 1;
//		wchar_t* wa = new wchar_t[size];
//		size_t outSize;
//#ifdef _WIN32
//		// retval , dst, dstsz, scr, len
//		mbstowcs_s(&outSize, wa, size, text, size - 1);
//#else
//		// dst, src, dstsz or len
//		outSize = mbstowcs(wa, text, size - 1);
//		outSize;
//#endif
//		return wa;
//	}

	void Debug::SetConsoleColor(const unsigned short aColor)
	{
#ifdef _WIN32
		HANDLE hConsole = nullptr;
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		if (hConsole != nullptr)
		{
			SetConsoleTextAttribute(hConsole, aColor);
		}
#else
		SUPRESS_UNUSED_WARNING(aColor);
#endif
	}

	wchar_t * Debug::ParseVariadicArgs(wchar_t aBuffer[], const char* aFormattedString, ...)
	{
		char buffer[MAX_STRING_BUFFER_SIZE] = {};
		va_list args;
		va_start(args, aFormattedString);
		VSPRINTF(buffer, aFormattedString, args);
		va_end(args);

		std::string charStr = buffer;
		std::wstring wcharStr(charStr.begin(), charStr.end());

		(void)memcpy_s(aBuffer, MAX_STRING_BUFFER_SIZE, wcharStr.c_str(), MAX_STRING_BUFFER_SIZE);

		return aBuffer;
	}

	wchar_t* Debug::ParseVariadicArgs(wchar_t aBuffer[], const wchar_t * aFormattedString, ...)
	{
		wchar_t buffer[MAX_STRING_BUFFER_SIZE] = {};
		va_list args;
		va_start(args, aFormattedString);
		wvsprintf(buffer, aFormattedString, args);
		va_end(args);

		(void)memcpy_s(aBuffer, MAX_STRING_BUFFER_SIZE, buffer, MAX_STRING_BUFFER_SIZE);

		return aBuffer;
	}

	void Log::CreateLog(const char* aFilePath)
	{
		myIsActive = false;
		myFile = aFilePath;
	}

	void Log::Write(const char * aMessage)
	{
		if (myIsActive == false)
			return;

		myDebugFile.open(myFile, std::ios::app);

		time_t rawTime;
		struct tm timeinfo;
		char timebuffer[80];
		time(&rawTime);
		localtime_s(&timeinfo, &rawTime);
		strftime(timebuffer, 80, "%d-%m-%Y %H:%M:%S", &timeinfo);
		
		myDebugFile << timebuffer << "	" << aMessage << std::endl;

		myDebugFile.close();
	}
}