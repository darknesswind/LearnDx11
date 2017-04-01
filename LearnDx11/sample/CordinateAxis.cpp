#include "stdafx.h"
#include "CordinateAxis.h"
#include "LD3DApplication.h"

CordinateAxis::CordinateAxis(LD3DApplication* pApp)
	: LSampleBase(pApp)
{
}

CordinateAxis::~CordinateAxis()
{
}

void CordinateAxis::createVertexBuf()
{
	m_verties = 
	{
		{ DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT4(DirectX::Colors::Red) },
		{ DirectX::XMFLOAT3(10, 0, 0), DirectX::XMFLOAT4(DirectX::Colors::Red) },
		{ DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT4(DirectX::Colors::Green) },
		{ DirectX::XMFLOAT3(0, 10, 0), DirectX::XMFLOAT4(DirectX::Colors::Green) },
		{ DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT4(DirectX::Colors::Blue) },
		{ DirectX::XMFLOAT3(0, 0, 10), DirectX::XMFLOAT4(DirectX::Colors::Blue) }
	};

	D3D11_BUFFER_DESC vbd = { 0 };
	fillBufDesc(vbd, sizeof(Vertex) * m_verties.size(), D3D11_BIND_VERTEX_BUFFER);

	D3D11_SUBRESOURCE_DATA srd = { m_verties.data() };
	m_pDevice->CreateBuffer(&vbd, &srd, &m_spVertexBuff);
}

void CordinateAxis::createIndexBuf()
{
}

void CordinateAxis::draw()
{
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(0.25f * DirectX::XM_PI, 4 / 3.f, 1.0f, 1000.0f);
	DirectX::XMMATRIX m = m_pApp->camera()->matrix() * proj;

	m_pApp->effect()->GetConstantBufferByIndex(0)->GetMemberByIndex(0)->AsMatrix()->SetMatrix(reinterpret_cast<float*>(&m));
	m_pApp->effect()->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, m_pContext);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_pContext->IASetVertexBuffers(0, 1, m_spVertexBuff.get(), &stride, &offset);
	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	m_pContext->Draw(m_verties.size(), 0);
}
