// LearnDx11.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TestVector.h"
#include "TestMatrix.h"
#include "LD3DApplication.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	LD3DApplication app(hInstance);
	app.init();
	return app.exec();
}