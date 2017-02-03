#include "stdafx.h"
#include "LCamera.h"


LCamera::LCamera()
{
	DirectX::XMMATRIX m = DirectX::XMMatrixIdentity();
	DirectX::XMStoreFloat4x4(&m_view, m);


	m_target = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	m_position = m_target;
	m_up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
}

LCamera::~LCamera()
{
}

void LCamera::setPosition(float radius, float phi, float theta)
{
	// Convert Spherical to Cartesian coordinates.
	float x = radius * sinf(phi) * cosf(theta);
	float z = radius * sinf(phi) * sinf(theta);
	float y = radius * cosf(phi);

	m_position = DirectX::XMVectorSet(x, y, z, 1.0f);
	m_view = DirectX::XMMatrixLookAtLH(m_position, m_target, m_up);
}

void LCamera::rotate(DirectX::FXMVECTOR RotationQuaternion)
{
	m_position = DirectX::XMVector3Rotate(m_position, RotationQuaternion);
	m_view = DirectX::XMMatrixLookAtLH(m_position, m_target, m_up);
}
