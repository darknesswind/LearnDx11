#pragma once
#include "LUnifiedTimer.h"
#include "LCamera.h"

class LMainWindow;
class LD3DDevice;
class LInput;
class LSamples;

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
	LMainWindow* mainWindow() { return m_spMainWnd.get(); }
	LCamera* camera() { return &m_camera; }
	LD3DDevice* device() { return m_spDevice.get(); }
	ID3DX11Effect* effect() { return m_spEffect; }


protected:
	virtual void update();
	virtual void draw();

protected:
	HINSTANCE m_hInstance;
	LUnifiedTimer m_unifiedTimer;
	std::unique_ptr<LMainWindow> m_spMainWnd;
	std::unique_ptr<LD3DDevice> m_spDevice;
	std::unique_ptr<LInput> m_spInput;

	com_ptr<ID3DX11Effect> m_spEffect;

	LCamera m_camera;
	std::unique_ptr<LSamples> m_spSamples;
};

