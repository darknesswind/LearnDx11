#pragma once
#include "LSampleBase.h"

class LD3DApplication;

class ColorBoxSample : public LSampleBase
{
public:
	ColorBoxSample(LD3DApplication* pApp);
	~ColorBoxSample();

public:
	void createInputLayout() override;
	void createVertexBuf() override;
	void createIndexBuf() override;
	void draw() override;

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;
};

