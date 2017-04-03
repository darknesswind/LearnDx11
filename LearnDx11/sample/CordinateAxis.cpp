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
		{ DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT4(DirectX::Colors::Red) },
		{ DirectX::XMFLOAT3(10, 0, 0), DirectX::XMFLOAT4(DirectX::Colors::Red) },
		{ DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT4(DirectX::Colors::Green) },
		{ DirectX::XMFLOAT3(0, 10, 0), DirectX::XMFLOAT4(DirectX::Colors::Green) },
		{ DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT4(DirectX::Colors::Blue) },
		{ DirectX::XMFLOAT3(0, 0, 10), DirectX::XMFLOAT4(DirectX::Colors::Blue) }
	};
}
