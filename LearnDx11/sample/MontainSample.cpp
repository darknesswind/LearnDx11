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
