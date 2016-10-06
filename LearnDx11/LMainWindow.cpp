#include "stdafx.h"
#include "LMainWindow.h"
#include "LD3DApplication.h"

#define WndClassName L"LiteDx11Window"

bool LMainWindow::s_bQuitFlag = false;;

LMainWindow::LMainWindow(LD3DApplication* pApp)
	: m_pApp(pApp)
{
	m_size.cx = 640;
	m_size.cy = 480;
}


LMainWindow::~LMainWindow()
{
}

void LMainWindow::setup()
{
	registerClass();
	createWindow();

	SetWindowLongW(m_hwnd, GWL_USERDATA, reinterpret_cast<LONG>(this));
	ShowWindow(m_hwnd, SW_SHOW);
// 	UpdateWindow(m_hwnd);
}

bool LMainWindow::processMessage()
{
	MSG msg;
	while (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return !s_bQuitFlag;
}

bool LMainWindow::registerClass()
{
	WNDCLASSW cls = { 0 };
	cls.style = 0;
	cls.lpfnWndProc = LMainWindow::WndProc;
	cls.cbClsExtra = 0;
	cls.cbWndExtra = 0;
	cls.hInstance = m_pApp->instance();
	cls.hIcon = NULL;
	cls.hCursor = NULL;
	cls.hbrBackground = NULL;
	cls.lpszMenuName = NULL;
	cls.lpszClassName = WndClassName;

	ATOM res = RegisterClassW(&cls);
	return (NULL != res);
}

bool LMainWindow::createWindow()
{
	m_hwnd = CreateWindowW(
		WndClassName, L"LiteSTG",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT,
		m_size.cx, m_size.cy,
		NULL, NULL,
		m_pApp->instance(), NULL);

	return (NULL != m_hwnd);
}

LRESULT LMainWindow::onMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_ACTIVATE:
		if (LOWORD(wparam) == WA_INACTIVE)
			m_pApp->pause();
		else
			m_pApp->resume();
		break;
	case WM_SIZE:
		if (SIZE_RESTORED == wparam)
			onResize();
		break;
	case WM_ENTERSIZEMOVE:
		m_pApp->pause();
		break;
	case WM_EXITSIZEMOVE:
		m_pApp->resume();
		onResize();
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_QUIT:
		s_bQuitFlag = true;
		break;
	}
	return DefWindowProcW(m_hwnd, msg, wparam, lparam);
}

void LMainWindow::onResize()
{
	RECT rc = { 0 };
	BOOL bSucc = GetWindowRect(m_hwnd, &rc);
	if (bSucc)
	{
		m_size.cx = rc.right - rc.left;
		m_size.cy = rc.bottom - rc.top;
		m_pApp->device()->onResize();
	}
}

LRESULT LMainWindow::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	LONG userDat = GetWindowLongW(hwnd, GWL_USERDATA);
	LMainWindow* pWindow = reinterpret_cast<LMainWindow*>(userDat);
	if (!pWindow)
		return DefWindowProcW(hwnd, msg, wparam, lparam);
	else
		return pWindow->onMessage(msg, wparam, lparam);
}
