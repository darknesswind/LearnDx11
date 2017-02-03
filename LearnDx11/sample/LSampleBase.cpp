#include "stdafx.h"
#include "LSampleBase.h"
#include "LD3DApplication.h"

LSampleBase::LSampleBase(LD3DApplication* pApp)
	: m_pApp(pApp)
{
}


LSampleBase::~LSampleBase()
{
}

void LSampleBase::createInputLayout()
{

}

void LSampleBase::createVertexBuf()
{

}

void LSampleBase::createIndexBuf()
{

}

void LSampleBase::draw()
{

}

void LSampleBase::fillBufDesc(D3D11_BUFFER_DESC& desc, UINT size, D3D11_BIND_FLAG bindFlag)
{
	desc.ByteWidth = size;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = bindFlag;
	desc.CPUAccessFlags = 0;// D3D11_CPU_ACCESS_FLAG
	desc.MiscFlags = 0;		// D3D11_RESOURCE_MISC_FLAG
	desc.StructureByteStride = 0;
}
