#include "stdafx.h"
#include "LD3DApplication.h"
#include "LMainWindow.h"
#include "LD3DDevice.h"
#include "LInput.h"
#include "sample/LSamples.h"
#include "sample/LSampleBase.h"
#include "sample/CordinateAxis.h"

LD3DApplication::LD3DApplication(HINSTANCE hInstance)
	: m_hInstance(hInstance)
{
}


LD3DApplication::~LD3DApplication()
{
}

bool LD3DApplication::init()
{
	m_spMainWnd = std::make_unique<class LMainWindow>(this);
	m_spDevice = std::make_unique<class LD3DDevice>(this);
	m_spInput = std::make_unique<class LInput>();

	m_spMainWnd->setup();
	if (!m_spDevice->setup())
		return false;

// 	m_camera.setPosition(5.0f, .25f * DirectX::XM_PI, 1.5f * DirectX::XM_PI);
	m_camera.setPosition(5.0f, 0.25*DirectX::XM_PI, 0.25*DirectX::XM_PI);

	CKHR(D3DX11CreateEffectFromFile(L"effect.fxo", 0, m_spDevice->device(), &m_spEffect));

	m_spSamples.reset(new LSamples(this));
	m_spSamples->init();
	return true;
}

int LD3DApplication::exec()
{
	std::unique_ptr<CordinateAxis> spAxis = std::make_unique<CordinateAxis>(this);
	spAxis->createInputLayout();
	spAxis->createVertexBuf();
	spAxis->createIndexBuf();

	m_spSamples->select(1);
	m_unifiedTimer.start();
	while (m_spMainWnd->processMessage())
	{
		m_spInput->update();

		m_spSamples->activeSample()->draw();
		spAxis->draw();
		update();
		draw();

		m_spDevice->swap();
		m_unifiedTimer.update();
	}

	m_spInput.release();
	m_spDevice.release();
	m_spMainWnd.release();
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
	if (m_spInput->keyDown(DirectX::Keyboard::Left))
	{
		DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(m_camera.up(), .01f);
		m_camera.rotate(q);
	}
	else if (m_spInput->keyDown(DirectX::Keyboard::Right))
	{
		DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(m_camera.up(), -.01f);
		m_camera.rotate(q);
	}

	if (m_spInput->keyDown(DirectX::Keyboard::Up))
	{
		DirectX::XMVECTOR axis = DirectX::XMVector3Cross(m_camera.up(), m_camera.lookat());
		if (DirectX::XMVector3Equal(axis, DirectX::XMVectorZero()))
			axis = DirectX::XMVectorSet(1.f, 0, 0, 0);
		DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(axis, .01f);
		m_camera.rotate(q);
	}
	else if (m_spInput->keyDown(DirectX::Keyboard::Down))
	{
		DirectX::XMVECTOR axis = DirectX::XMVector3Cross(m_camera.up(), m_camera.lookat());
		if (DirectX::XMVector3Equal(axis, DirectX::XMVectorZero()))
			axis = DirectX::XMVectorSet(1.f, 0, 0, 0);
		DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(axis, -.01f);
		m_camera.rotate(q);
	}

}

void LD3DApplication::draw()
{
}
