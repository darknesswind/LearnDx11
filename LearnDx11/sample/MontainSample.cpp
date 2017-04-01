#include "stdafx.h"
#include "MontainSample.h"
#include "LD3DApplication.h"

MontainSample::MontainSample(LD3DApplication* pApp)
	: LSampleBase(pApp)
{
}


MontainSample::~MontainSample()
{
}

void MontainSample::createVertexBuf()
{
	m_verties.resize((s_xCnt + 1) * (s_zCnt + 1));
	size_t pos = 0;
	for (size_t r = 0; r <= s_zCnt; ++r)
	{
		for (size_t c = 0; c <= s_xCnt; ++c, ++pos)
		{
			Vertex& vertex = m_verties[pos];
			float x = 4.f * c / s_xCnt - 2.f;
			float z = 4.f * r / s_zCnt - 2.f;
			float y = std::sin(x * z) + std::sin(x + z);
			vertex.pos = DirectX::XMFLOAT3(x, y, z);
			float tint = y / 2.0f + .5f;
			vertex.clr = DirectX::XMFLOAT4(tint, tint, tint, 1.0f);
		}
	}

	D3D11_BUFFER_DESC vbd = { 0 };
	fillBufDesc(vbd, sizeof(Vertex) * m_verties.size(), D3D11_BIND_VERTEX_BUFFER);

	D3D11_SUBRESOURCE_DATA srd = { m_verties.data() };
	m_pDevice->CreateBuffer(&vbd, &srd, &m_spVertexBuff);
}

void MontainSample::createIndexBuf()
{
	m_indices.resize((s_xCnt + 1) * (s_zCnt + 1) * 2 * 3);
	size_t* pData = m_indices.data();
	for (size_t r = 0; r < s_zCnt; ++r)
	{
		for (size_t c = 0; c < s_xCnt; ++c)
		{
			size_t lt = (r + 1) * (s_xCnt + 1) + c;
			size_t lb = r * (s_xCnt + 1) + c;
			size_t rt = lt + 1;
			size_t rb = lb + 1;

			pData[0] = lt;
			pData[1] = rt;
			pData[2] = lb;

			pData[3] = rt;
			pData[4] = rb;
			pData[5] = lb;

			pData += 6;
		}
	}

	D3D11_BUFFER_DESC ibd = { 0 };
	fillBufDesc(ibd, sizeof(size_t) * m_indices.size(), D3D11_BIND_INDEX_BUFFER);

	D3D11_SUBRESOURCE_DATA srd = { m_indices.data() };
	m_pDevice->CreateBuffer(&ibd, &srd, &m_spIndexBuff);
}

void MontainSample::draw()
{
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(0.25f * DirectX::XM_PI, 4 / 3.f, 1.0f, 1000.0f);
	DirectX::XMMATRIX m = m_pApp->camera()->matrix() * proj;

	m_pApp->effect()->GetConstantBufferByIndex(0)->GetMemberByIndex(0)->AsMatrix()->SetMatrix(reinterpret_cast<float*>(&m));
	m_pApp->effect()->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, m_pContext);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_pContext->IASetVertexBuffers(0, 1, m_spVertexBuff.get(), &stride, &offset);
	m_pContext->IASetIndexBuffer(m_spIndexBuff, DXGI_FORMAT_R32_UINT, 0);
	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pContext->DrawIndexed(m_indices.size(), 0, 0);
}
