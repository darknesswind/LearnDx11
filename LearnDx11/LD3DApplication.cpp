#include "stdafx.h"
#include "LD3DApplication.h"


LD3DApplication::LD3DApplication(HINSTANCE hInstance)
	: m_hInstance(hInstance)
	, m_mainWnd(this)
	, m_device(this)
{
}


LD3DApplication::~LD3DApplication()
{
}

bool LD3DApplication::init()
{
	m_mainWnd.setup();
	if (!m_device.setup())
		return false;

	return true;
}

int LD3DApplication::exec()
{
	m_device.test_createInputLayout();
	m_unifiedTimer.start();
	while (m_mainWnd.processMessage())
	{
		m_device.test_createBuff();
		update();
		draw();

		m_device.swap();
		m_unifiedTimer.update();
	}
	return 0;
}

void LD3DApplication::pause()
{

}

void LD3DApplication::resume()
{

}

void LD3DApplication::update()
{

}

void LD3DApplication::draw()
{
}
