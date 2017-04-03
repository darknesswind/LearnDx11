#include "stdafx.h"
#include "ShapesSample.h"


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
	createCylinder(cylinder, mesh);

	Sphere sphere = { 0 };
	sphere.radius = 1.f;
	sphere.stack = 8;
	sphere.slice = 16;
	createSphere(sphere, mesh);
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
