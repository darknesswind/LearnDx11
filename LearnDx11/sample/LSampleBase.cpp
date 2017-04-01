#include "stdafx.h"
#include "LSampleBase.h"
#include "LD3DApplication.h"
#include "LD3DDevice.h"

LSampleBase::LSampleBase(LD3DApplication* pApp)
	: m_pApp(pApp)
{
	LD3DDevice* pDevice = m_pApp->device();
	m_pDevice = pDevice->device();
	m_pContext = pDevice->immContext();
}


LSampleBase::~LSampleBase()
{
}

void LSampleBase::createInputLayout()
{
	D3DX11_PASS_DESC passDesc = { 0 };
	m_pApp->effect()->GetTechniqueByIndex(0)->GetPassByIndex(0)->GetDesc(&passDesc);

	D3D11_INPUT_ELEMENT_DESC desc1[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	com_ptr<ID3D11InputLayout> spLayout;
	CKHR(m_pDevice->CreateInputLayout(desc1, ARRAYSIZE(desc1),
		passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &spLayout));
	m_pContext->IASetInputLayout(spLayout);

	com_ptr<ID3D11RasterizerState> spState;
	D3D11_RASTERIZER_DESC desc;
	memset(&desc, 0, sizeof(desc));
	bool bBorder = false;
	if (bBorder)
	{
		desc.FillMode = D3D11_FILL_WIREFRAME;
		desc.CullMode = D3D11_CULL_NONE;
	}
	else
	{
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;
	}

	desc.DepthClipEnable = TRUE;

	m_pDevice->CreateRasterizerState(&desc, &spState);
	m_pContext->RSSetState(spState);
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
