#include "stdafx.h"
#include "ShapesSample.h"
#include <unordered_map>

ShapesSample::ShapesSample(LD3DApplication* pApp)
	: LSampleBase(pApp)
{
	m_fillMode = D3D11_FILL_WIREFRAME;
// 	m_cullMode = D3D11_CULL_NONE;
}

ShapesSample::~ShapesSample()
{
}

void ShapesSample::createModel(MeshData& mesh)
{
	Cylinder cylinder = { 0 };
	cylinder.topRadius = 0.2f;
	cylinder.bottomRadius = 0.8f;
	cylinder.height = 2.0f;
	cylinder.slice = 10;
	cylinder.stack = 4;
// 	createCylinder(cylinder, mesh);

	Sphere sphere = { 0 };
	sphere.radius = 1.f;
	sphere.stack = 8;
	sphere.slice = 16;
// 	createSphere(sphere, mesh);

	Geosphere sphere2 = { 0 };
	sphere2.radius = 2;
	sphere2.divLevel = 2;
	createGeosphere(sphere2, mesh);
}

std::vector<Vector3> ShapesSample::createCircelNormal(size_t slice)
{
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0, 1, 0, 0);
	DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(up, DirectX::XM_2PI / slice);

	std::vector<Vector3> normal(slice);
	normal[0] = Vector3(1, 0, 0);
	for (size_t i = 1; i < normal.size(); ++i)
	{
		normal[i] = DirectX::XMVector3Rotate(normal[i - 1], q);
	}
	for (size_t i = 1; i < normal.size(); ++i)
	{
		normal[i] = DirectX::XMVector3Normalize(normal[i]);
	}
	return normal;
}

void ShapesSample::createCircelTri(Indices& indices, size_t iCenter, size_t iFrom, size_t iTo)
{
	if (iFrom < iTo)
	{
		for (size_t i = iFrom; i < iTo; ++i)
		{
			indices.push_back(iCenter);
			indices.push_back(i);
			indices.push_back(i + 1);
		}
	}
	else
	{
		for (size_t i = iTo; i < iFrom; ++i)
		{
			indices.push_back(iCenter);
			indices.push_back(i + 1);
			indices.push_back(i);
		}
	}
	indices.push_back(iCenter);
	indices.push_back(iTo);
	indices.push_back(iFrom);
}

void ShapesSample::createCylindricalTri(Indices& indices, size_t iFrom, size_t stack, size_t slice)
{
	size_t base = iFrom;
	for (size_t i = 0; i < stack; ++i)
	{
		for (size_t j = 0; j < slice; ++j)
		{
			size_t rt = base + j;
			size_t rb = rt + slice;
			size_t lt = rt + 1;
			if (j + 1 == slice)
				lt = base;
			size_t lb = lt + slice;

			indices.push_back(rt);
			indices.push_back(rb);
			indices.push_back(lb);

			indices.push_back(rt);
			indices.push_back(lb);
			indices.push_back(lt);
		}
		base += slice;
	}
}

void ShapesSample::createCylinder(const Cylinder& shape, MeshData& mesh)
{
	if (!shape.isValid())
		return;

	std::vector<Vector3> normal = createCircelNormal(shape.slice);
	float hStep = shape.height / shape.stack;
	float rStrp = (shape.bottomRadius - shape.topRadius) / shape.stack;
	// create vertex
	size_t beginIdx = mesh.vertices.size();
	Vertex v = { DirectX::XMFLOAT3(0, shape.height, 0), DirectX::XMFLOAT4(DirectX::Colors::White) };
	mesh.vertices.push_back(v);
	float r = shape.topRadius;
	float h = shape.height;
	for (size_t i = 0; i < shape.stack; ++i)
	{
		for each (const Vector3& p in normal)
		{
			v.pos = p * r;
			v.pos.y = h;
			mesh.vertices.push_back(v);
		}
		r += rStrp;
		h -= hStep;
	}
	for each (const Vector3& p in normal)
	{
		v.pos = p * shape.bottomRadius;
		v.pos.y = 0;
		mesh.vertices.push_back(v);
	}
	v.pos = DirectX::XMFLOAT3(0, 0, 0);
	mesh.vertices.push_back(v);

	// create index
	createCircelTri(mesh.indices, beginIdx, beginIdx + 1, beginIdx + shape.slice);
	createCylindricalTri(mesh.indices, beginIdx + 1, shape.stack, shape.slice);
	size_t base = mesh.vertices.size() - 1;
	createCircelTri(mesh.indices, base, base - 1, base - shape.slice);
}

void ShapesSample::createSphere(const Sphere& shape, MeshData& mesh)
{
	std::vector<Vector3> normal = createCircelNormal(shape.slice);

	size_t beginIdx = mesh.vertices.size();
	Vertex v = { DirectX::XMFLOAT3(0, shape.radius, 0), DirectX::XMFLOAT4(DirectX::Colors::White) };
	mesh.vertices.push_back(v);
	float alphaStep = DirectX::XM_PI / shape.stack;
	float alpha = 0;
	for (size_t i = 1; i < shape.stack; ++i)
	{
		alpha += alphaStep;
		float r = shape.radius * sin(alpha);
		float y = shape.radius * cos(alpha);
		for (size_t j = 0; j < normal.size(); ++j)
		{
			v.pos = r * normal[j];
			v.pos.y = y;
			mesh.vertices.push_back(v);
		}
	}
	v.pos = DirectX::XMFLOAT3(0, -shape.radius, 0);
	mesh.vertices.push_back(v);

	createCircelTri(mesh.indices, beginIdx, beginIdx + 1, beginIdx + shape.slice);
	createCylindricalTri(mesh.indices, beginIdx + 1, shape.stack - 2, shape.slice);
	size_t base = mesh.vertices.size() - 1;
	createCircelTri(mesh.indices, base, base - 1, base - shape.slice);
}

void ShapesSample::createGeosphere(const Geosphere& shape, MeshData& mesh)
{
	size_t vertexBegin = mesh.vertices.size();
	size_t indexBegin = mesh.indices.size();
	if (false)
	{
		const float sqrt3 = sqrt(3.f);
		mesh.vertices.insert(mesh.vertices.end(),
		{
			{ Vector3(0, 1, 0), DirectX::XMFLOAT4(DirectX::Colors::White) },
			{ Vector3(sqrt3 / 2, -0.5f, 0), DirectX::XMFLOAT4(DirectX::Colors::White) },
			{ Vector3(-sqrt3 / 4, -0.5f, 3.f / 4), DirectX::XMFLOAT4(DirectX::Colors::White) },
			{ Vector3(-sqrt3 / 4, -0.5f, -3.f / 4), DirectX::XMFLOAT4(DirectX::Colors::White) }
		});
		mesh.indices.insert(mesh.indices.end(),
		{
			vertexBegin, vertexBegin + 1, vertexBegin + 3,
			vertexBegin, vertexBegin + 2, vertexBegin + 1,
			vertexBegin, vertexBegin + 3, vertexBegin + 2,
			vertexBegin + 1, vertexBegin + 2, vertexBegin + 3
		});
	}
	else
	{
		const int slice = 5;
		const float rad36 = 36.0 * DirectX::XM_PI / 180.0;
		const float fsin36 = 1.f / sin(rad36);
		const float ftan36 = 1.f / tan(rad36);
		const float bodyheight = sqrt(3 - (fsin36 - ftan36) * (fsin36 - ftan36)) / 2;
		const float topheight = sqrt(4 - fsin36 * fsin36) + bodyheight;

		Vertex v = { DirectX::XMFLOAT3(0, topheight, 0), DirectX::XMFLOAT4(DirectX::Colors::White) };
		mesh.vertices.push_back(v);
		std::vector<Vector3> normal = createCircelNormal(slice);
		for (size_t i = 0; i < normal.size(); ++i)
		{
			normal[i] *= fsin36;
			normal[i].y = bodyheight;
			v.pos = normal[i];
			mesh.vertices.push_back(v);
			normal[i] = -normal[i];
		}
		for (size_t i = 0; i < normal.size(); ++i)
		{
			size_t pos = (i + 2) % normal.size();
			v.pos = normal[pos];
			mesh.vertices.push_back(v);
		}
		v.pos = DirectX::XMFLOAT3(0, -topheight, 0);
		mesh.vertices.push_back(v);

		createCircelTri(mesh.indices, vertexBegin, vertexBegin + 1, vertexBegin + slice);
		createCylindricalTri(mesh.indices, vertexBegin + 1, 1, slice);
		size_t base = mesh.vertices.size() - 1;
		createCircelTri(mesh.indices, base, base - 1, base - slice);
	}
	for (size_t i = 0; i < shape.divLevel; ++i)
	{
		subdivGeosphere(mesh, indexBegin, mesh.indices.size());
	}
	for (size_t i = vertexBegin; i < mesh.vertices.size(); ++i)
	{ 
		 DirectX::XMStoreFloat3(&mesh.vertices[i].pos, 
			 DirectX::XMVectorScale(
				 DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&mesh.vertices[i].pos)),
				 shape.radius));
	}
}

void ShapesSample::subdivGeosphere(MeshData& mesh, size_t indexBegin, size_t indexEnd)
{
	struct IndexHelper
	{
		union Key
		{
			struct
			{
				size_t iFrom;
				size_t iTo;
			};
			__int64 raw;
			operator const __int64& () const { return raw; }
		};
		std::unordered_map<Key, size_t, std::hash<__int64>> m_map;
		Vertices& vertices;

		IndexHelper(MeshData& mesh) : vertices(mesh.vertices) {}
		size_t getMiddle(size_t iFrom, size_t iTo)
		{
			if (iFrom == iTo)
				return iFrom;

			Key key{ iFrom, iTo };
			if (key.iFrom > key.iTo)
				std::swap(key.iFrom, key.iTo);
			auto iter = m_map.find(key);
			if (iter != m_map.end())
				return iter->second;

			size_t newIdx = vertices.size();
			Vector3 newPos = (vertices[iFrom].pos + vertices[iTo].pos) / 2;
			vertices.emplace_back(Vertex{ newPos, DirectX::XMFLOAT4(DirectX::Colors::White) });
			m_map[key] = newIdx;
			return newIdx;
		}
	} helper(mesh);
	for (size_t i = indexBegin; i < indexEnd; i += 3)
	{
		size_t* pTri = &mesh.indices[i];
		size_t base[3] = { pTri[0], pTri[1], pTri[2] };
		size_t subdiv[3] = // 影响顶点容器
		{
			helper.getMiddle(base[0], base[1]),
			helper.getMiddle(base[1], base[2]),
			helper.getMiddle(base[0], base[2])
		};
		// 先填中间
		memcpy(pTri, subdiv, sizeof(subdiv));
		mesh.indices.insert(mesh.indices.end(), 
		{
			base[0], subdiv[0], subdiv[2],
			base[1], subdiv[1], subdiv[0],
			base[2], subdiv[2], subdiv[1]
		});
	}
}

bool Cylinder::isValid() const
{
	if (topRadius <= 0 || bottomRadius <= 0 || height <= 0)
		return false;

	if (slice < 2 || stack < 1)
		return false;

	return true;
}

size_t Cylinder::vertexCount() const
{
	return 2 + (stack + 1) * slice;
}
