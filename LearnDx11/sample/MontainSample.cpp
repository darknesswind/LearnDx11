#include "stdafx.h"
#include "MontainSample.h"
#include "LD3DApplication.h"
#include "LD3DDevice.h"

MontainSample::MontainSample(LD3DApplication* pApp)
	: LSampleBase(pApp)
{
	m_vertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
}


MontainSample::~MontainSample()
{
}

void MontainSample::update()
{
	static float cnt = 0;
	cnt += 0.01f;
	ID3D11DeviceContext* pContext = m_pApp->device()->immContext();

	D3D11_MAPPED_SUBRESOURCE res = { 0 };
	CKHR(pContext->Map(m_spVertexBuff, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &res));
	// 用 DISCARD 模式时, buff中的数据应视为无效
	Vertex* pVertics = reinterpret_cast<Vertex*>(res.pData);
	for (size_t i = 0; i < m_vertexSize; ++i)
	{
		DirectX::XMFLOAT3& pos = pVertics[i].pos;
		pos.y = (std::sin(pos.x * pos.z + cnt) + std::sin(pos.x + pos.z - cnt)) / 2;

		float tint = pos.y / 2.0f + .5f;
		pVertics[i].clr = DirectX::XMFLOAT4(tint, tint, tint, 1.0f);
	}

	pContext->Unmap(m_spVertexBuff, 0);
}

void MontainSample::createModel(MeshData& mesh)
{
	mesh.vertices.resize((s_xCnt + 1) * (s_zCnt + 1));
	size_t pos = 0;
	for (size_t r = 0; r <= s_zCnt; ++r)
	{
		for (size_t c = 0; c <= s_xCnt; ++c, ++pos)
		{
			Vertex& vertex = mesh.vertices[pos];
			float x = 4.f * c / s_xCnt - 2.f;
			float z = 4.f * r / s_zCnt - 2.f;
			float y = std::sin(x * z) + std::sin(x + z);
			vertex.pos = DirectX::XMFLOAT3(x, y, z);
			float tint = y / 2.0f + .5f;
			vertex.clr = DirectX::XMFLOAT4(tint, tint, tint, 1.0f);
		}
	}

	mesh.indices.resize((s_xCnt + 1) * (s_zCnt + 1) * 2 * 3);
	size_t* pData = mesh.indices.data();
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
}
