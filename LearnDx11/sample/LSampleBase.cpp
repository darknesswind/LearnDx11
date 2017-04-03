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

void LSampleBase::create()
{
	createInputLayout();

	MeshData mesh;
	createModel(mesh);
	m_vertexSize = mesh.vertices.size();
	m_indexSize = mesh.indices.size();

	D3D11_BUFFER_DESC vbd = { 0 };
	fillBufDesc(vbd, sizeof(Vertices::value_type) * m_vertexSize, D3D11_BIND_VERTEX_BUFFER);

	D3D11_SUBRESOURCE_DATA srd = { mesh.vertices.data() };
	m_pDevice->CreateBuffer(&vbd, &srd, &m_spVertexBuff);

	if (!mesh.indices.empty())
	{
		D3D11_BUFFER_DESC ibd = { 0 };
		fillBufDesc(ibd, sizeof(Indices::value_type) * m_indexSize, D3D11_BIND_INDEX_BUFFER);

		D3D11_SUBRESOURCE_DATA srd = { mesh.indices.data() };
		m_pDevice->CreateBuffer(&ibd, &srd, &m_spIndexBuff);
	}
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

	desc.FillMode = m_fillMode;
	desc.CullMode = m_cullMode;
	desc.DepthClipEnable = TRUE;

	m_pDevice->CreateRasterizerState(&desc, &spState);
	m_pContext->RSSetState(spState);
}

void LSampleBase::createModel(MeshData& mesh)
{

}

void LSampleBase::draw()
{
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(0.25f * DirectX::XM_PI, 4 / 3.f, 1.0f, 1000.0f);
	DirectX::XMMATRIX m = m_pApp->camera()->matrix() * proj;

	m_pApp->effect()->GetConstantBufferByIndex(0)->GetMemberByIndex(0)->AsMatrix()->SetMatrix(reinterpret_cast<float*>(&m));
	m_pApp->effect()->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, m_pContext);
	m_pContext->IASetPrimitiveTopology(m_topology);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_pContext->IASetVertexBuffers(0, 1, m_spVertexBuff.get(), &stride, &offset);
	if (m_indexSize > 0)
	{
		m_pContext->IASetIndexBuffer(m_spIndexBuff, DXGI_FORMAT_R32_UINT, 0);
		m_pContext->DrawIndexed(m_indexSize, 0, 0);
	}
	else
	{
		m_pContext->Draw(m_vertexSize, 0);
	}
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
