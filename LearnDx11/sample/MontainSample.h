#pragma once
#include "LSampleBase.h"

class MontainSample : public LSampleBase
{
public:
	MontainSample(LD3DApplication* pApp);
	~MontainSample();

public:
	void createModel(MeshData& mesh) override;

private:
	const static size_t s_xCnt = 40;
	const static size_t s_zCnt = 40;
};

