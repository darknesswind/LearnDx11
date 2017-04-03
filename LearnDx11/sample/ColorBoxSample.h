#pragma once
#include "LSampleBase.h"

class LD3DApplication;

class ColorBoxSample : public LSampleBase
{
public:
	ColorBoxSample(LD3DApplication* pApp);
	~ColorBoxSample();

public:
	void createModel(MeshData& mesh) override;

private:
};

