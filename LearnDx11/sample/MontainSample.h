#pragma once
#include "LSampleBase.h"

class MontainSample : public LSampleBase
{
public:
	MontainSample(LD3DApplication* pApp);
	~MontainSample();

public:
	void createVertexBuf() override;
	void createIndexBuf() override;
	void draw() override;

private:
	const static size_t s_xCnt = 40;
	const static size_t s_zCnt = 40;

	std::vector<Vertex> m_verties;
	std::vector<size_t> m_indices;
};

