#include "stdafx.h"
#include "DebugInfoDrawer.h"

#include "TextInstance.h"
#include "..\CommonUtilities\CountDown.h"
#include "..\CommonUtilities\TimerManager.h"
#include "..\PostMaster\PostMaster.h"
#include "..\PostMaster\Message.h"
#include "..\PostMaster\Event.h"
#include "..\CommonUtilities\EKeyboardKeys.h"
#include "Engine.h"

#include <Psapi.h>
#include "EDebugInfoDrawerFlags.h"

//TEMP INCLUDES
#include "..\PostMaster\ChangeLevel.h"

CDebugInfoDrawer::CDebugInfoDrawer(unsigned int aDebugFlags)
	: myOutputTexts(nullptr)
	, myCountDown(nullptr)
	, myRenderThreadTimers(nullptr)
	, myFPSTimer(0)
	, myUpdateTextTimer(0)
	, myBlinkTimer(0)
	, myDrawCallsCount(0)
	, myDebugFlags(aDebugFlags)
	, myLeftControlIsDown(false)
{
#ifndef _RETAIL_BUILD

	myOutputTexts[eDebugText_FPS] = new CTextInstance();
	myOutputTexts[eDebugText_FPS]->Init();
	myOutputTexts[eDebugText_FPS]->SetText("FPS: ");

	myOutputTexts[eDebugText_DrawCalls] = new CTextInstance();
	myOutputTexts[eDebugText_DrawCalls]->Init();
	myOutputTexts[eDebugText_DrawCalls]->SetText("DRAWCALLS: ");

	myOutputTexts[eDebugText_MemoryUsage] = new CTextInstance();
	myOutputTexts[eDebugText_MemoryUsage]->Init();
	myOutputTexts[eDebugText_MemoryUsage]->SetText("MEMORY (MB): ");

	myCountDown = new CU::CountDown();

	myRenderThreadTimers = new CU::TimerManager();

	myFPSTimer = myRenderThreadTimers->CreateTimer();
	myUpdateTextTimer = myRenderThreadTimers->CreateTimer();

	PostMaster::GetInstance().AppendSubscriber(this, eMessageType::eKeyboardMessage);
	PostMaster::GetInstance().AppendSubscriber(this, eMessageType::eDrawCallsThisFrame);

#endif // !_RETAIL_BUILD
}

CDebugInfoDrawer::~CDebugInfoDrawer()
{
#ifndef _RETAIL_BUILD
	SAFE_DELETE(myRenderThreadTimers);
	SAFE_DELETE(myCountDown);

	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eKeyboardMessage);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eDrawCallsThisFrame);

	myOutputTexts.DeleteAll();
#endif // !_RETAIL_BUILD
}

void CDebugInfoDrawer::Render(const CU::Vector2ui aTargetWindowSize)
{
#ifndef _RETAIL_BUILD

	const CU::Vector2f targetWindowSize(/*1920.f, 1080.f*/aTargetWindowSize);
	const float xStartPos = 2.f / targetWindowSize.x;
	const float yStartPos = 2.f / targetWindowSize.y;
	const float yOffset = 25.f / targetWindowSize.y;

	int yCount = 0;

	for (int i = 0; i < myOutputTexts.Size(); ++i)
	{
		if (myDebugFlags & (1 << i))
		{
			myOutputTexts[i]->SetPosition({ xStartPos, yStartPos + yOffset * yCount++ });
			myOutputTexts[i]->Render();
		}
	}

	const CU::Timer& blinkTimer = myRenderThreadTimers->GetTimer(myBlinkTimer);
	const float BlinkTime = 0.5f;
	if (blinkTimer.GetLifeTime().GetSeconds() < BlinkTime * 2.f || true)
	{
		//if (blinkTimer.GetLifeTime().GetSeconds() < BlinkTime)
		{
			myCountDown->Render();
		}
	}
	else
	{
		myRenderThreadTimers->ResetTimer(myBlinkTimer);
	}

#endif // !_RETAIL_BUILD
}

void CDebugInfoDrawer::Update()
{
#ifndef _RETAIL_BUILD
	myRenderThreadTimers->UpdateTimers();

	UpdateFPSCounter();
	UpdateDrawCallsCounter();
	UpdateMemoryUsage();

	myCountDown->Update();

#endif // !_RETAIL_BUILD
}

eMessageReturn CDebugInfoDrawer::Recieve(const Message & aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}

void CDebugInfoDrawer::PressedKey(const CU::eKeys& aKey)
{
	SUPRESS_UNUSED_WARNING(aKey);

#ifndef _RETAIL_BUILD

	switch (aKey)
	{
	case CU::eKeys::LCONTROL:
		myLeftControlIsDown = true;
		break;
	case CU::eKeys::SEVEN:
		if (myLeftControlIsDown == true)
		{
			myDebugFlags ^= eDebugFlags_FPS;
		}
		break;
	case CU::eKeys::EIGHT:
		if (myLeftControlIsDown == true)
		{
			myDebugFlags ^= eDebugFlags_DrawCalls;
		}
		break;
	case CU::eKeys::NINE:
		if (myLeftControlIsDown == true)
		{
			myDebugFlags ^= eDebugFlags_MemoryUsage;
		}
		break;
	default:
		break;
	}

#endif // !_RETAIL_BUILD
}

void CDebugInfoDrawer::ReleasedKey(const CU::eKeys& aKey)
{
	switch (aKey)
	{
	case CU::eKeys::LCONTROL:
		myLeftControlIsDown = false;
		break;
	default:
		break;
	}
}

void CDebugInfoDrawer::SetDrawCalls(const int aDrawCallsCount)
{
	myDrawCallsCount = aDrawCallsCount;
}

void CDebugInfoDrawer::UpdateFPSCounter()
{
#ifndef _RETAIL_BUILD
	const float SecondsBetweenRefreshes = 0.125f;

	if (myRenderThreadTimers->GetTimer(myUpdateTextTimer).GetLifeTime().GetSeconds() > SecondsBetweenRefreshes)
	{
		myRenderThreadTimers->ResetTimer(myUpdateTextTimer);

		const int currentFPS = static_cast<int>(myRenderThreadTimers->GetTimer(myFPSTimer).GetFPS());
		myOutputTexts[eDebugText_FPS]->SetText(CU::DynamicString("FPS: ") + currentFPS);

		if (currentFPS < 30)
		{
			myOutputTexts[eDebugText_FPS]->SetColor(CU::Vector4f(CTextInstance::Red.x * 0.5f, CTextInstance::Red.y * 0.5f, CTextInstance::Red.z * 0.5f, 1.f));
		}
		else if (currentFPS < 60)
		{
			myOutputTexts[eDebugText_FPS]->SetColor(CU::Vector4f(CTextInstance::Yellow.x * 0.5f, CTextInstance::Yellow.y * 0.5f, CTextInstance::Yellow.z * 0.5f, 1.f));
		}
		else
		{
			myOutputTexts[eDebugText_FPS]->SetColor(CU::Vector4f(CTextInstance::White.x * 0.5f, CTextInstance::White.y * 0.5f, CTextInstance::White.z * 0.5f, 1.f));
		}
	}
#endif // !_RETAIL_BUILD
}

void CDebugInfoDrawer::UpdateMemoryUsage()
{
	PROCESS_MEMORY_COUNTERS memCounter = {};
	BOOL result = GetProcessMemoryInfo(GetCurrentProcess(), &memCounter, sizeof(memCounter));
	if (result == TRUE)
	{
		int memUsedkB = static_cast<int>(memCounter.WorkingSetSize / 1024);
		int memUsedMB = static_cast<int>(memCounter.WorkingSetSize / 1024 / 1024);
		if (memUsedkB < 1000)
		{
			myOutputTexts[eDebugText_MemoryUsage]->SetText(CU::DynamicString("MEMORY (kB): ") + memUsedkB);
		}
		else
		{
			myOutputTexts[eDebugText_MemoryUsage]->SetText(CU::DynamicString("MEMORY (MB): ") + memUsedMB);
		}

		if (memUsedMB > 1500)
		{
			myOutputTexts[eDebugText_MemoryUsage]->SetColor(CU::Vector4f(CTextInstance::Red.x * 0.5f, CTextInstance::Red.y * 0.5f, CTextInstance::Red.z * 0.5f, 1.f));
		}
		else if (memUsedMB > 1000)
		{
			myOutputTexts[eDebugText_MemoryUsage]->SetColor(CU::Vector4f(CTextInstance::Yellow.x * 0.5f, CTextInstance::Yellow.y * 0.5f, CTextInstance::Yellow.z * 0.5f, 1.f));
		}
		else
		{
			myOutputTexts[eDebugText_MemoryUsage]->SetColor(CU::Vector4f(CTextInstance::White.x * 0.5f, CTextInstance::White.y * 0.5f, CTextInstance::White.z * 0.5f, 1.f));
		}
	}
}

void CDebugInfoDrawer::UpdateDrawCallsCounter()
{
	myOutputTexts[eDebugText_DrawCalls]->SetText(CU::DynamicString("DRAWCALLS: ") + myDrawCallsCount);
	if (myDrawCallsCount > 2000)
	{
		myOutputTexts[eDebugText_DrawCalls]->SetColor(CU::Vector4f(CTextInstance::Red.x * 0.5f, CTextInstance::Red.y * 0.5f, CTextInstance::Red.z * 0.5f, 1.f));
	}
	else if (myDrawCallsCount > 1000)
	{
		myOutputTexts[eDebugText_DrawCalls]->SetColor(CU::Vector4f(CTextInstance::Yellow.x * 0.5f, CTextInstance::Yellow.y * 0.5f, CTextInstance::Yellow.z * 0.5f, 1.f));
	}
	else
	{
		myOutputTexts[eDebugText_DrawCalls]->SetColor(CU::Vector4f(CTextInstance::White.x * 0.5f, CTextInstance::White.y * 0.5f, CTextInstance::White.z * 0.5f, 1.f));
	}
}
