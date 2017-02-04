#include "stdafx.h"

#include "ColorBoxSample.h"
#include "LD3DApplication.h"

ColorBoxSample::ColorBoxSample(LD3DApplication* pApp)
	: LSampleBase(pApp)
{
}


ColorBoxSample::~ColorBoxSample()
{
}

void ColorBoxSample::createVertexBuf()
{
	Vertex vertics[] =
	{
		{ DirectX::XMFLOAT3(-1.f, -1.f, -1.f), DirectX::XMFLOAT4(DirectX::Colors::White) },
		{ DirectX::XMFLOAT3(-1.f, +1.f, -1.f), DirectX::XMFLOAT4(DirectX::Colors::Black) },
		{ DirectX::XMFLOAT3(+1.f, +1.f, -1.f), DirectX::XMFLOAT4(DirectX::Colors::Red) },
		{ DirectX::XMFLOAT3(+1.f, -1.f, -1.f), DirectX::XMFLOAT4(DirectX::Colors::Green) },
		{ DirectX::XMFLOAT3(-1.f, -1.f, +1.f), DirectX::XMFLOAT4(DirectX::Colors::Blue) },
		{ DirectX::XMFLOAT3(-1.f, +1.f, +1.f), DirectX::XMFLOAT4(DirectX::Colors::Yellow) },
		{ DirectX::XMFLOAT3(+1.f, +1.f, +1.f), DirectX::XMFLOAT4(DirectX::Colors::Cyan) },
		{ DirectX::XMFLOAT3(+1.f, -1.f, +1.f), DirectX::XMFLOAT4(DirectX::Colors::Magenta) },
	};

	D3D11_BUFFER_DESC vbd = { 0 };
	fillBufDesc(vbd, sizeof(vertics), D3D11_BIND_VERTEX_BUFFER);

	D3D11_SUBRESOURCE_DATA srd = { vertics };

	com_ptr<ID3D11Buffer> spBuff;
	m_pDevice->CreateBuffer(&vbd, &srd, &spBuff);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_pContext->IASetVertexBuffers(0, 1, spBuff.get(), &stride, &offset);
}

UINT g_indices[] =
{
	0, 1, 2,
	0, 2, 3,
	4, 6, 5,
	4, 7, 6,
	4, 5, 1,
	4, 1, 0,
	3, 2, 6,
	3, 6, 7,
	1, 5, 6,
	1, 6, 2,
	4, 0, 3,
	4, 3, 7,
};

void ColorBoxSample::createIndexBuf()
{
	D3D11_BUFFER_DESC ibd = { 0 };
	fillBufDesc(ibd, sizeof(g_indices), D3D11_BIND_INDEX_BUFFER);

	D3D11_SUBRESOURCE_DATA srd = { g_indices };

	com_ptr<ID3D11Buffer> spBuff;
	m_pDevice->CreateBuffer(&ibd, &srd, &spBuff);
	m_pContext->IASetIndexBuffer(spBuff, DXGI_FORMAT_R32_UINT, 0);
}

void ColorBoxSample::draw()
{
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(0.25f * DirectX::XM_PI, 4 / 3.f, 1.0f, 1000.0f);
	DirectX::XMMATRIX m = m_pApp->camera()->matrix() * proj;

	m_pApp->effect()->GetConstantBufferByIndex(0)->GetMemberByIndex(0)->AsMatrix()->SetMatrix(reinterpret_cast<float*>(&m));
	m_pApp->effect()->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, m_pContext);

	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pContext->DrawIndexed(ARRAYSIZE(g_indices), 0, 0);
}
