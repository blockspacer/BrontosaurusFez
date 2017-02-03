#include "stdafx.h"
#include "StopWatch.h"

namespace CU
{
	CStopWatch::CStopWatch()
		: myLifeTime(0.f)
		, myDeltaTime(0.f)
	{
	}

	CStopWatch::~CStopWatch()
	{
	}

	void CStopWatch::Update()
	{
		TimePoint newTimePoint = Clock::now();
		FloatDuration rawDeltaTime = myLastTimePoint - newTimePoint;
		MicroSeconds deltaTime = std::chrono::duration_cast<MicroSeconds>(rawDeltaTime);

		myDeltaTime = deltaTime.count();
		myLifeTime += myDeltaTime;

		myLastTimePoint = newTimePoint;
	}
}
