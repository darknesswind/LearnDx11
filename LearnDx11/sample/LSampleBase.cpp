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

	m_vertexDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	m_indexDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
}


LSampleBase::~LSampleBase()
{
}

void LSampleBase::create()
{
	createInputLayout();

	MeshData mesh;
	createModel(mesh);
	if (m_topology == D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
		calcTriListNormal(mesh);
	
	m_vertexSize = mesh.vertices.size();
	m_indexSize = mesh.indices.size();

	m_vertexDesc.ByteWidth = sizeof(Vertices::value_type) * m_vertexSize;
	if (m_vertexDesc.CPUAccessFlags != 0 && m_vertexDesc.Usage == D3D11_USAGE_IMMUTABLE)
		m_vertexDesc.Usage = D3D11_USAGE_DYNAMIC;

	D3D11_SUBRESOURCE_DATA srd = { mesh.vertices.data() };
	m_pDevice->CreateBuffer(&m_vertexDesc, &srd, &m_spVertexBuff);

	if (!mesh.indices.empty())
	{
		m_indexDesc.ByteWidth = sizeof(Indices::value_type) * m_indexSize;
		if (m_indexDesc.CPUAccessFlags != 0 && m_indexDesc.Usage == D3D11_USAGE_IMMUTABLE)
			m_indexDesc.Usage = D3D11_USAGE_DYNAMIC;

		D3D11_SUBRESOURCE_DATA srd = { mesh.indices.data() };
		m_pDevice->CreateBuffer(&m_indexDesc, &srd, &m_spIndexBuff);
	}

	ID3DX11EffectConstantBuffer* pcbPerFrame = m_pApp->effect()->GetConstantBufferByName("cbPerFrame");
	if (pcbPerFrame)
	{
		m_gTime = pcbPerFrame->GetMemberByName("gTime");
	}
	ID3DX11EffectConstantBuffer* pcbPerObject = m_pApp->effect()->GetConstantBufferByName("cbPerObject");
	if (pcbPerObject)
	{
		ID3DX11EffectVariable* pVar = pcbPerObject->GetMemberByName("gWorldViewProj");
		if (pVar) m_gWorldViewProj = pVar->AsMatrix();
	}
}

void LSampleBase::update()
{

}

void LSampleBase::createInputLayout()
{
	D3DX11_PASS_DESC passDesc = { 0 };
	m_pApp->effect()->GetTechniqueByIndex(0)->GetPassByIndex(0)->GetDesc(&passDesc);

	D3D11_INPUT_ELEMENT_DESC desc1[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TARGET",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	1, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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
// 	desc.ScissorEnable = TRUE;
	m_pDevice->CreateRasterizerState(&desc, &spState);
	m_pContext->RSSetState(spState);

	if (desc.ScissorEnable)
	{
		D3D11_RECT rect = { 100, 100, 400, 400 };
		m_pContext->RSSetScissorRects(1, &rect);
	}
}

void LSampleBase::createModel(MeshData& mesh)
{

}

void LSampleBase::calcTriListNormal(MeshData & mesh)
{
	for (size_t i = 0; i < mesh.indices.size(); i += 3)
	{
		size_t idx0 = mesh.indices[i];
		size_t idx1 = mesh.indices[i + 1];
		size_t idx2 = mesh.indices[i + 2];
		Vector3& p0 = mesh.vertices[idx0].pos;
		Vector3& p1 = mesh.vertices[idx1].pos;
		Vector3& p2 = mesh.vertices[idx2].pos;
		Vector3 normal = (p1 - p0).Cross(p2 - p0);

		mesh.vertices[idx0].normal += normal;
		mesh.vertices[idx1].normal += normal;
		mesh.vertices[idx2].normal += normal;
	}
	for (auto iter = mesh.vertices.begin(); iter != mesh.vertices.end(); ++iter)
		iter->normal.Normalize();
}

void LSampleBase::draw()
{
	if (m_gTime)
	{
		float nFrame = m_pApp->timer().curFrame() / 60.0f;
		m_gTime->SetRawValue(&nFrame, 0, sizeof(nFrame));
	}
	if (m_gWorldViewProj)
	{
		DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(0.25f * DirectX::XM_PI, 4 / 3.f, 1.0f, 1000.0f);
		DirectX::XMMATRIX m = m_pApp->camera()->matrix() * proj;
		m_gWorldViewProj->SetMatrix(reinterpret_cast<float*>(&m));
	}
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
