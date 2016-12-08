#pragma once
#include <string>
#include "resource.h"

struct SInitWindowParams;

class CWindowsWindow
{
public:
	CWindowsWindow(SInitWindowParams& aInitWindowParams);
	~CWindowsWindow();
	void Update();
	void Close();
	inline bool GetIsWindowOpen();
	inline HWND GetHWND();
	inline HINSTANCE GetHinstance();

	const CU::Vector2ui& GetWindowSize();

private:
	ATOM MyRegisterClass(HINSTANCE hInstance, std::wstring windowName);
	BOOL InitInstance(SInitWindowParams& aInitWindowParams);
	HINSTANCE myHInst;
	HWND myHWnd;
	MSG myMsg;
	bool isWindowOpen = true;
};

inline bool CWindowsWindow::GetIsWindowOpen()
{
	return isWindowOpen;
}

inline HWND CWindowsWindow::GetHWND()
{
	return myHWnd;
}

inline HINSTANCE CWindowsWindow::GetHinstance()
{
	return myHInst;
}