// LearnDx11.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TestVector.h"
#include "TestMatrix.h"
#include "LD3DDevice.h"
#include "LMainWindow.h"

int messageLoop()
{
	MSG msg;
	while (GetMessageW(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return static_cast<int>(msg.wParam);
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	LMainWindow wnd(hInstance);
	wnd.setup();

	LD3DDevice device;
	if (!device.init(wnd.handle()))
		return -1;

	return messageLoop();
}