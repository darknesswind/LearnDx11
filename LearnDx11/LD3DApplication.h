#pragma once
#include "LMainWindow.h"
#include "LD3DDevice.h"
#include "LUnifiedTimer.h"

class LD3DApplication
{
public:
	LD3DApplication(HINSTANCE hInstance);
	~LD3DApplication();

	bool init();
	int exec();
	void pause();
	void resume();

	HINSTANCE instance() const { return m_hInstance; }
	LMainWindow* mainWindow() { return &m_mainWnd; }
	LD3DDevice* device() { return &m_device; }

protected:
	virtual void update();
	virtual void draw();

protected:
	HINSTANCE m_hInstance;
	LMainWindow m_mainWnd;
	LD3DDevice m_device;
	LUnifiedTimer m_unifiedTimer;
};

