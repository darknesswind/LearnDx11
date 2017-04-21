#include "stdafx.h"
#include "CordinateAxis.h"
#include "LD3DApplication.h"

CordinateAxis::CordinateAxis(LD3DApplication* pApp)
	: LSampleBase(pApp)
{
	m_topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
}

CordinateAxis::~CordinateAxis()
{
}

void CordinateAxis::createModel(MeshData& mesh)
{
	mesh.vertices = 
	{
		{ Vector3(0, 0, 0), DirectX::XMFLOAT4(DirectX::Colors::Red) },
		{ Vector3(10, 0, 0), DirectX::XMFLOAT4(DirectX::Colors::Red) },
		{ Vector3(0, 0, 0), DirectX::XMFLOAT4(DirectX::Colors::Green) },
		{ Vector3(0, 10, 0), DirectX::XMFLOAT4(DirectX::Colors::Green) },
		{ Vector3(0, 0, 0), DirectX::XMFLOAT4(DirectX::Colors::Blue) },
		{ Vector3(0, 0, 10), DirectX::XMFLOAT4(DirectX::Colors::Blue) }
	};
}
