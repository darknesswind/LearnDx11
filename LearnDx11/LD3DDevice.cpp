#include "stdafx.h"
#include <DirectXColors.h>

#include "LD3DDevice.h"
#include "LMainWindow.h"
#include "LD3DApplication.h"

LD3DDevice::LD3DDevice(LD3DApplication* pApp)
	: m_pApp(pApp)
	, m_pWindow(nullptr)
	, m_bUse4xMSAA(false)
	, m_clrFormat(DXGI_FORMAT_R8G8B8A8_UNORM)
{
}


LD3DDevice::~LD3DDevice()
{
}

bool LD3DDevice::setup()
{
	if (!DirectX::XMVerifyCPUSupport())
	{
		std::cout << "not support DirectX Match" << std::endl;
		return false;
	}
	m_pWindow = m_pApp->mainWindow();
	assert(m_pWindow);

	if (!createDevice())
		return false;

	CKHR(getFactory(m_spDevice, &m_spFactory));
	if (!m_spFactory)
		return false;

#ifdef _DEBUG
	printDeviceInfo();
#endif

	if (!createSwapChain())
		return false;

	CKHR(m_spFactory->MakeWindowAssociation(m_pWindow->handle(), DXGI_MWA_NO_WINDOW_CHANGES));

	if (!createRenderTargetView())
		return false;

	if (!createDepthStencilView())
		return false;

	createViewport();
	return true;
}

void LD3DDevice::swap()
{
	m_spImmContext->ClearRenderTargetView(m_spTargetView, DirectX::Colors::Black);
	m_spImmContext->ClearDepthStencilView(m_spDepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	CKHR(m_spSwapChin->Present(0, 0));
}

bool LD3DDevice::onResize()
{
	m_spDepthView.clear();
	m_spTargetView.clear();
// 	m_spSwapChin.clear();

// 	if (!createSwapChain())
// 		return false;

	if (!m_spSwapChin)
		return false;

	if (!createRenderTargetView())
		return false;

	if (!createDepthStencilView())
		return false;

	return true;
}

bool LD3DDevice::createDevice()
{
	const UINT levelCount = 4;
	D3D_FEATURE_LEVEL featureLevels[levelCount] =
	{
		D3D_FEATURE_LEVEL_11_0, // First check D3D 11 support
		D3D_FEATURE_LEVEL_10_1, // Second check D3D 10.1 support
		D3D_FEATURE_LEVEL_10_0, // Next, check D3D 10 support
		D3D_FEATURE_LEVEL_9_3, // Finally, check D3D 9.3 support
	};

	UINT flags = 0;
#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	
	D3D_FEATURE_LEVEL supportLevel = D3D_FEATURE_LEVEL_11_0;
	HRESULT hr = CKHR(D3D11CreateDevice(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, flags,
		featureLevels, levelCount, D3D11_SDK_VERSION, &m_spDevice, &supportLevel, &m_spImmContext));

	if (D3D_FEATURE_LEVEL_11_0 != supportLevel)
	{
		std::cout << "Not support DirectX 11!" << std::endl;
	}
	if (FAILED(hr))
	{
		std::cout << "Create device failed!" << std::endl;
		return false;
	}
	return true;
}

bool LD3DDevice::createSwapChain()
{
	if (!m_spDevice)
		return false;

	DXGI_SWAP_CHAIN_DESC chainDesc = { 0 };
	fillSwapChainDesc(m_pWindow->handle(), chainDesc);

	CKHR(m_spFactory->CreateSwapChain(m_spDevice, &chainDesc, &m_spSwapChin));
	return (nullptr != m_spSwapChin);
}

bool LD3DDevice::createRenderTargetView()
{
	if (!m_spSwapChin)
		return false;

	com_ptr<ID3D11Texture2D> spTexture;
	CKHR(m_spSwapChin->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&spTexture));
	if (spTexture)
		CKHR(m_spDevice->CreateRenderTargetView(spTexture, nullptr, &m_spTargetView));

	return (nullptr != m_spTargetView);
}

bool LD3DDevice::createDepthStencilView()
{
	if (!m_spTargetView)
		return false;

	D3D11_TEXTURE2D_DESC desc;
	fillDepthTexture2DDesc(desc);

	com_ptr<ID3D11Texture2D> spTexture;
	HRESULT hr = CKHR(m_spDevice->CreateTexture2D(&desc, NULL, &spTexture));
	if (!spTexture)
		return false;

	hr = CKHR(m_spDevice->CreateDepthStencilView(spTexture, nullptr, &m_spDepthView));
	if (!m_spDepthView)
		return false;

	m_spImmContext->OMSetRenderTargets(1, m_spTargetView.getAddress(), m_spDepthView);
	return true;
}

void LD3DDevice::createViewport()
{
	D3D11_VIEWPORT viewport = { 0 };
	viewport.Width = m_pWindow->width();
	viewport.Height = m_pWindow->height();
	viewport.MaxDepth = 1;
	m_spImmContext->RSSetViewports(1, &viewport);
}

void LD3DDevice::fillSwapChainDesc(HWND hwnd, DXGI_SWAP_CHAIN_DESC& chainDesc)
{
	DXGI_MODE_DESC& buff = chainDesc.BufferDesc;
	buff.Width = m_pWindow->width();
	buff.Height = m_pWindow->height();
	buff.RefreshRate.Numerator = 60;
	buff.RefreshRate.Denominator = 1;
	buff.Format = m_clrFormat;
	buff.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	buff.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	fillSampleDesc(chainDesc.SampleDesc);

	chainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	chainDesc.BufferCount = 1;
	chainDesc.OutputWindow = hwnd;
	chainDesc.Windowed = TRUE;
	chainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	chainDesc.Flags = 0;
}

void LD3DDevice::fillSampleDesc(DXGI_SAMPLE_DESC& desc)
{
	static UINT quality = 0;
	if (0 == quality && m_spDevice)
	{
		HRESULT hr = CKHR(m_spDevice->CheckMultisampleQualityLevels(m_clrFormat, 4, &quality));
		assert(quality > 0);
	}
	if (m_bUse4xMSAA)
	{
		desc.Count = 4;
		desc.Quality = quality;
	}
	else
	{
		desc.Count = 1;
		desc.Quality = 0;
	}
}

void LD3DDevice::fillDepthTexture2DDesc(D3D11_TEXTURE2D_DESC& desc)
{
	desc.Width = m_pWindow->width();
	desc.Height = m_pWindow->height();
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	fillSampleDesc(desc.SampleDesc);

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
}

void LD3DDevice::fillVertexBuffDesc(D3D11_BUFFER_DESC& desc, UINT size)
{
	desc.ByteWidth = size;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;// D3D11_CPU_ACCESS_FLAG
	desc.MiscFlags = 0;		// D3D11_RESOURCE_MISC_FLAG
	desc.StructureByteStride = 0;
}

HRESULT LD3DDevice::getFactory(ID3D11Device* pDevice, IDXGIFactory** ppFactory)
{
	com_ptr<IDXGIDevice> spGIDevice = pDevice;
	com_ptr<IDXGIAdapter> spAdapter;
	HRESULT hr = CKHR(spGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&spAdapter));
	if (FAILED(hr))
		return hr;

	return spAdapter->GetParent(__uuidof(IDXGIFactory), (void**)ppFactory);
}

void LD3DDevice::printDeviceInfo()
{
	if (!m_spFactory)
		return;

	HRESULT hr = S_OK;
	UINT idxAdapter = 0;
	while (SUCCEEDED(hr))
	{
		com_ptr<IDXGIAdapter> spAdapter;
		hr = m_spFactory->EnumAdapters(idxAdapter, &spAdapter);
		++idxAdapter;
		if (!spAdapter)
			continue;

		DXGI_ADAPTER_DESC adapterDesc = { 0 };
		spAdapter->GetDesc(&adapterDesc);
		OutputDebugStringW(adapterDesc.Description);
		OutputDebugStringW(L"\n");

		UINT idxOutput = 0;
		HRESULT hr2 = S_OK;
		while (SUCCEEDED(hr2))
		{
			com_ptr<IDXGIOutput> spOutput;
			hr2 = spAdapter->EnumOutputs(idxOutput, &spOutput);
			++idxOutput;
			if (!spOutput)
				continue;

			DXGI_OUTPUT_DESC outputDesc = { 0 };
			spOutput->GetDesc(&outputDesc);
			OutputDebugStringW(outputDesc.DeviceName);
			OutputDebugStringW(L"\n");

			UINT cnt = 0;
			spOutput->GetDisplayModeList(m_clrFormat, DXGI_ENUM_MODES_INTERLACED, &cnt, NULL);
			std::vector<DXGI_MODE_DESC> modes(cnt);
			spOutput->GetDisplayModeList(m_clrFormat, DXGI_ENUM_MODES_INTERLACED, &cnt, modes.data());
			WCHAR buf[256];
			UINT prevW = 0, prevH = 0, prevR = 0;
			for (size_t i = 0; i < cnt; ++i)
			{
				UINT refRate = static_cast<UINT>(round(1.0 * modes[i].RefreshRate.Numerator / modes[i].RefreshRate.Denominator));
				if (prevW == modes[i].Width && prevH == modes[i].Height && prevR == refRate)
					continue;
				prevW = modes[i].Width;
				prevH = modes[i].Height;
				prevR = refRate;

				wsprintf(buf, L"%dx%d %dHz\n", modes[i].Width, modes[i].Height, refRate);
				OutputDebugStringW(buf);
			}
		}
		OutputDebugStringW(L"\n");
	}
}

void LD3DDevice::test_createInputLayout()
{
	CKHR(D3DX11CreateEffectFromFile(L"effect.fxo", 0, m_spDevice, &m_spEffect));

	D3DX11_PASS_DESC passDesc = { 0 };
	m_spEffect->GetTechniqueByIndex(0)->GetPassByIndex(0)->GetDesc(&passDesc);

	D3D11_INPUT_ELEMENT_DESC desc1[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	com_ptr<ID3D11InputLayout> spLayout;
	CKHR(m_spDevice->CreateInputLayout(desc1, ARRAYSIZE(desc1),
		passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &spLayout));
	m_spImmContext->IASetInputLayout(spLayout);
}

void LD3DDevice::test_createBuff()
{
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 clr;
	};
	Vertex vertics[] =
	{
		{ DirectX::XMFLOAT3(-1.f, -1.f, -1.f), DirectX::XMFLOAT4(DirectX::Colors::White) },
		{ DirectX::XMFLOAT3(-1.f, 1.f, -1.f), DirectX::XMFLOAT4(DirectX::Colors::Black) },
		{ DirectX::XMFLOAT3(1.f, 1.f, -1.f), DirectX::XMFLOAT4(DirectX::Colors::Red) },
		{ DirectX::XMFLOAT3(1.f, -1.f, -1.f), DirectX::XMFLOAT4(DirectX::Colors::Green) },
		{ DirectX::XMFLOAT3(-1.f, -1.f, 1.f), DirectX::XMFLOAT4(DirectX::Colors::Blue) },
		{ DirectX::XMFLOAT3(-1.f, 1.f, 1.f), DirectX::XMFLOAT4(DirectX::Colors::Yellow) },
		{ DirectX::XMFLOAT3(1.f, 1.f, 1.f), DirectX::XMFLOAT4(DirectX::Colors::Cyan) },
		{ DirectX::XMFLOAT3(1.f, -1.f, 1.f), DirectX::XMFLOAT4(DirectX::Colors::Magenta) },
	};

	{{
		D3D11_BUFFER_DESC vbd = { 0 };
		fillVertexBuffDesc(vbd, sizeof(vertics));

		D3D11_SUBRESOURCE_DATA srd = { vertics };

		com_ptr<ID3D11Buffer> spBuff;
		m_spDevice->CreateBuffer(&vbd, &srd, &spBuff);
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		m_spImmContext->IASetVertexBuffers(0, 1, spBuff.getAddress(), &stride, &offset);
	}}
	UINT indices[24] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 5,
		0, 5, 6,
		0, 6, 7,
		0, 7, 8,
		0, 8, 1
	};
	{{
		D3D11_BUFFER_DESC ibd = { 0 };
		fillVertexBuffDesc(ibd, sizeof(indices));
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA srd = { indices };

		com_ptr<ID3D11Buffer> spBuff;
		m_spDevice->CreateBuffer(&ibd, &srd, &spBuff);
		m_spImmContext->IASetIndexBuffer(spBuff, DXGI_FORMAT_R32_UINT, 0);
	}}
	m_spImmContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DirectX::XMFLOAT4X4 m4 =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 0
	};

	m_spEffect->GetConstantBufferByIndex(0)->GetMemberByIndex(0)->AsMatrix()->SetMatrixTranspose((float*)&m4.m);
	m_spEffect->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, m_spImmContext);
	m_spImmContext->DrawIndexed(ARRAYSIZE(indices), 0, 0);
}
