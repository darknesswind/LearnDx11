#pragma once
#include "LSampleBase.h"

struct Cylinder;
struct Sphere;
struct Geosphere;
class ShapesSample : public LSampleBase
{
public:
	ShapesSample(LD3DApplication* pApp);
	~ShapesSample();

	virtual void createModel(MeshData& mesh) override;

	static std::vector<Vector3> createCircelNormal(size_t slice);
	static void createCircelTri(Indices& indices, size_t iCenter, size_t iFrom, size_t iTo);
	static void createCylindricalTri(Indices& indices, size_t iFrom, size_t stack, size_t slice);
	static void createCylinder(const Cylinder& shape, MeshData& mesh);
	static void createSphere(const Sphere& shape, MeshData& mesh);
	static void createGeosphere(const Geosphere& shape, MeshData& mesh);
	static void subdivGeosphere(MeshData& mesh, size_t vertexBegin, size_t indexBegin);
};

struct Cylinder
{
	float topRadius;
	float bottomRadius;
	float height;
	size_t slice;
	size_t stack;

	bool isValid() const;
	size_t vertexCount() const;
};

struct Sphere
{
	float radius;
	size_t slice;
	size_t stack;
};

struct Geosphere
{
	float radius;
	size_t divLevel;
};