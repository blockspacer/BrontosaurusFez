#include "stdafx.h"
#include "WindowsWindow.h"
#include "Engine.h"
#include "../PostMaster/PostMaster.h"
#include "../PostMaster/FocusChange.h"
#include "../PostMaster/Message.h"

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

CWindowsWindow::CWindowsWindow(const SInitWindowParams& aInitWindowParams)
{
	InitInstance(aInitWindowParams);
}

CWindowsWindow::~CWindowsWindow()
{
}

void CWindowsWindow::Update()
{
	while (PeekMessage(&myMsg, myHWnd, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&myMsg);
		DispatchMessage(&myMsg);
	}
}

void CWindowsWindow::Close()
{
	isWindowOpen = false;
}



ATOM CWindowsWindow::MyRegisterClass(HINSTANCE hInstance, const wchar_t* windowName)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BRONTOSAURUS));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_BRONTOSAURUS);
	wcex.lpszClassName = windowName;

	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL CWindowsWindow::InitInstance(const SInitWindowParams& aInitWindowParams)
{
	myHInst = GetModuleHandle(0);

	MyRegisterClass(myHInst, aInitWindowParams.Name.c_str());
	
	myHWnd = CreateWindowW(aInitWindowParams.Name.c_str(), aInitWindowParams.Title.c_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, aInitWindowParams.Width, aInitWindowParams.Height, nullptr, nullptr, myHInst, nullptr);

	if (!myHWnd)
	{
		return FALSE;
	}

	ShowWindow(myHWnd, SW_SHOW); //5 = show normal
	UpdateWindow(myHWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:

		break;
	case WM_ENTERSIZEMOVE:

		break;

	case WM_EXITSIZEMOVE:
		RECT rect;
		GetClientRect(hWnd, &rect);
		CEngine::GetInstance()->OnResize(rect.right - rect.left, rect.bottom - rect.top);
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			hdc;
		}
		break;
	case WM_CREATE:

		break;
	case WM_DESTROY:
		CEngine::GetInstance()->Shutdown();
		PostQuitMessage(0);
		break;
	case WM_KILLFOCUS:
	{
		PostMaster* pm = PostMaster::GetInstancePtr();
		if (pm != nullptr)
		{
			pm->SendLetter(Message(eMessageType::eFokusChanged, FocusChange(false)));
		}
	}
		break;
	case WM_SETFOCUS:
	{
		PostMaster* pm = PostMaster::GetInstancePtr();
		if (pm != nullptr)
		{
			pm->SendLetter(Message(eMessageType::eFokusChanged, FocusChange(true)));
		}
	}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
