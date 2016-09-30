#include "stdafx.h"
#include "LMainWindow.h"

#define WndClassName L"LiteDx11Window"

LMainWindow::LMainWindow(HINSTANCE hIns)
	: m_hInstance(hIns)
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

	ShowWindow(m_hwnd, SW_SHOW);
// 	UpdateWindow(m_hwnd);
}

bool LMainWindow::registerClass()
{
	WNDCLASSW cls = { 0 };
	cls.style = 0;
	cls.lpfnWndProc = LMainWindow::WndProc;
	cls.cbClsExtra = 0;
	cls.cbWndExtra = 0;
	cls.hInstance = m_hInstance;
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
		WS_OVERLAPPED | WS_CAPTION,
		CW_USEDEFAULT, CW_USEDEFAULT,
		m_size.cx, m_size.cy,
		NULL, NULL,
		m_hInstance, NULL);

	return (NULL != m_hwnd);
}

LRESULT LMainWindow::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProcW(hwnd, msg, wparam, lparam);
}
