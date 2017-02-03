#pragma once
class LD3DApplication;

class LSampleBase
{
public:
	LSampleBase(LD3DApplication* pApp);
	virtual ~LSampleBase();

public:
	virtual void createInputLayout();
	virtual void createVertexBuf();
	virtual void createIndexBuf();
	virtual void draw();

protected:
	void fillBufDesc(D3D11_BUFFER_DESC& desc, UINT size, D3D11_BIND_FLAG bindFlag);

protected:
	LD3DApplication* m_pApp;

};

