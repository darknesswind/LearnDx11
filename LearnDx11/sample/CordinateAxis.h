#pragma once
#include "LSampleBase.h"

class CordinateAxis : public LSampleBase
{
public:
	CordinateAxis(LD3DApplication* pApp);
	~CordinateAxis();

public:
	void createModel(MeshData& mesh) override;

};

