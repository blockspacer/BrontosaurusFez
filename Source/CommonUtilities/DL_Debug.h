#pragma once
#include <stdlib.h>
#include <string>
#include <fstream>
#include <unordered_map>
#include "DL_Assert.h"

namespace DL_Debug
{
	const unsigned short CONSOLE_TEXT_COLOR_RED = 12;
	const unsigned short CONSOLE_TEXT_COLOR_YELLOW = 14;
	const unsigned short CONSOLE_TEXT_COLOR_GREEN = 10;
	const unsigned short CONSOLE_TEXT_COLOR_WHITE = 15;
}

//LOOKA HIERE!
#ifdef _WIN32
	#include <assert.h>
#else
	#ifdef NDEBUG
		#undef NDEBUG
		#include <assert.h>
		#define NDEBUG
	#else
		#include <assert.h>
	#endif
#endif

#define USE_FILTERLOG

#ifdef USE_FILTERLOG  
	#define LOG(aLogtype, ...) DL_WRITELOG((aLogtype), __VA_ARGS__)
	#define RESOURCES_LOG(...) DL_WRITELOG(DL_Debug::eLogTypes::eResource, __VA_ARGS__)
	#define ENGINE_LOG(...) DL_WRITELOG(DL_Debug::eLogTypes::eEngine, __VA_ARGS__)
	#define GAMEPLAY_LOG(...) DL_WRITELOG(DL_Debug::eLogTypes::eGamePlay, __VA_ARGS__)
#else //!USE_FILTERLOG
	#define LOG(logtype, ...)
	#define RESOURCES_LOG(...)
	#define ENGINE_LOG(...)
	#define GAMEPLAY_LOG(...)
#endif //USE_FILTERLOG

#ifndef _RETAIL_BUILD
//#define DL_ASSERT(string, ...) DL_Debug::Debug::GetInstance()->AssertMessage(__FILE__, __LINE__, __FUNCTION__, string, __VA_ARGS__)
#define DL_ASSERT(string, ...) wchar_t assertBuffer[1024]; _wassert(DL_Debug::Debug::GetInstance()->ParseVariadicArgs(assertBuffer, string, __VA_ARGS__), __FILEW__, __LINE__)
#define DL_PRINT(string, ...)  DL_Debug::Debug::GetInstance()->PrintMessage(string, __VA_ARGS__)
#define DL_PRINT_WARNING(message, ...) DL_Debug::Debug::GetInstance()->SetConsoleColor(DL_Debug::CONSOLE_TEXT_COLOR_YELLOW); DL_Debug::Debug::GetInstance()->PrintMessage(message, __VA_ARGS__); DL_Debug::Debug::GetInstance()->SetConsoleColor(DL_Debug::CONSOLE_TEXT_COLOR_WHITE)
#define DL_WRITELOG(aLogTypes, ...) DL_Debug::Debug::GetInstance()->WriteLog(aLogTypes, __VA_ARGS__); DL_PRINT(__VA_ARGS__)
#define DL_MESSAGE_BOX(message, ...) DL_Debug::Debug::GetInstance()->ShowMessageBox(message, __VA_ARGS__)
#define DL_FATAL_ERROR(string, ...) DL_ASSERT(__VA_ARGS__)
#else //!_RETAIL_BUILD
#define DL_ASSERT(string, ...) (string)
#define DL_PRINT(string, ...)  (string)
#define DL_PRINT_WARNING(string, ...) (string)
#define DL_WRITELOG(aLogTypes, ...) (aLogTypes)
#define DL_MESSAGE_BOX(message, ...) (message)
#define DL_FATAL_ERROR(string, ...) DL_Debug::Debug::GetInstance()->ShowMessageBox(message, __VA_ARGS__); exit(1)
#endif //RETAIL_BUILD

//#define DL_FATAL_ERROR(string, ...) DL_Debug::Debug::GetInstance()->AssertMessage(__FILE__, __LINE__, __FUNCTION__, string, __VA_ARGS__)

#define DL_DEBUG_INST DL_Debug::Debug::GetInstance()

namespace DL_Debug
{
	enum class eLogTypes
	{
		eResource,
		eEngine,
		eGamePlay,
		eCrash,
		eThreadedModels,
	};

	class Log
	{
	public:
		void CreateLog(const char* aFileName);
		inline void Activate(){ myIsActive = true; }
		inline void Deactivate() { myIsActive = false; }
		void Write(const char* aMessage);

	private:
		const char* myFile;
		std::ofstream myDebugFile;
		bool myIsActive;
	};

	class Debug
	{
	public:
		static bool Destroy();
		static Debug* GetInstance();

		void Activate(eLogTypes aLogType);
		void Deactivate(eLogTypes aLogType);

		void AssertMessage(const char *aFileName, const int aLine, const char *aFunctionName, const char *aString, ...);
		void AssertMessage(const char *aFileName, const int aLine, const char *aFunctionName, const wchar_t *aString, ...);
		void PrintMessage(const char *aString, ...);
		void PrintMessage(const wchar_t* aWString, ...);
		void WriteLog(eLogTypes aLogType, const char *aFormattedString, ...);
		void ShowMessageBox(const char* aMessage, ...);
		void ShowMessageBox(const wchar_t* aMessage, ...);

		//static wchar_t* charToWChar(const char* text);
		void SetConsoleColor(const unsigned short aColor);
		wchar_t* ParseVariadicArgs(wchar_t aBuffer[], const char* aFormattedString, ...);
		wchar_t* ParseVariadicArgs(wchar_t aBuffer[], const wchar_t* aFormattedString, ...);

	private:
		void CreateLog(eLogTypes aLogType);
		Debug();
		~Debug();
		static Debug* ourInstance;
		std::unordered_map<eLogTypes, Log> myLogFiles;
	};
}
