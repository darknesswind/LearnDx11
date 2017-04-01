#pragma once
#include "LSampleBase.h"

class CordinateAxis : public LSampleBase
{
public:
	CordinateAxis(LD3DApplication* pApp);
	~CordinateAxis();

public:
	void createVertexBuf() override;
	void createIndexBuf() override;
	void draw() override;

private:
	std::vector<Vertex> m_verties;
};

