#pragma once

#include <windows.h>
#include <thread>


namespace CU
{
	void SetThreadName(uint32_t dwThreadID, const char* threadName);
	void SetThreadName(const char* threadName);

	void SetThreadName(std::thread* thread, const char* threadName);
}