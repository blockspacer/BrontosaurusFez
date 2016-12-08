#pragma once

#include "../CommonUtilities/StaticArray.h"
#include "../PostMaster/Subscriber.h"

#include <atomic>

namespace CU
{
	class CountDown;

	template <typename T>
	class Vector2;
	using Vector2ui = Vector2<unsigned int>;

	class TimerManager;
	typedef unsigned int TimerHandle;

	enum class eKeys;
}

class CTextInstance;

class CDebugInfoDrawer : public Subscriber
{
public:
	CDebugInfoDrawer(unsigned int aDebugFlags = 0);
	~CDebugInfoDrawer();

	void Render(const CU::Vector2ui aTargetWindowSize);
	void Update();

	eMessageReturn Recieve(const Message& aMessage) override;

	void PressedKey(const CU::eKeys& aKey);
	void ReleasedKey(const CU::eKeys& aKey);

	void SetDrawCalls(const int aDrawCallsCount);

private:
	void UpdateFPSCounter();
	void UpdateDrawCallsCounter();
	void UpdateMemoryUsage();

	enum eDebugText
	{
		eDebugText_FPS,
		eDebugText_DrawCalls,
		eDebugText_MemoryUsage,
		eDebugText_Length,
	};

	CU::StaticArray<CTextInstance*, eDebugText_Length> myOutputTexts;

	CU::CountDown* myCountDown;

	CU::TimerManager* myRenderThreadTimers;
	CU::TimerHandle myFPSTimer;
	CU::TimerHandle myUpdateTextTimer;
	CU::TimerHandle myBlinkTimer;

	int myDrawCallsCount;
	std::atomic_uint32_t myDebugFlags;

	std::atomic_bool myLeftControlIsDown;
	//std::atomic_bool myShowFPS;
	//std::atomic_bool myShowDrawCalls;
	//std::atomic_bool myShowMemoryUsage;
};

