#include "stdafx.h"
#include "LD3DDevice.h"


LD3DDevice::LD3DDevice()
{
}


LD3DDevice::~LD3DDevice()
{
}

bool LD3DDevice::init(HWND hwnd)
{
	if (!DirectX::XMVerifyCPUSupport())
	{
		std::cout << "not support DirectX Match" << std::endl;
		return false;
	}

	DXGI_SWAP_CHAIN_DESC chainDesc = { 0 };
	fillSwapChainDesc(hwnd, chainDesc);

	if (!createDevice(chainDesc))
		return false;

	if (!createRenderTargetView())
		return false;

	return true;
}

bool LD3DDevice::createDevice(DXGI_SWAP_CHAIN_DESC& chainDesc)
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
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, flags,
		featureLevels, levelCount, D3D11_SDK_VERSION, &chainDesc,
		&m_spSwapChin, &m_spDevice, &supportLevel, &m_spImmContext);

	if (FAILED(hr))
	{
		std::cout << "Create device failed!" << std::endl;
		return false;
	}
	return true;
}

bool LD3DDevice::createRenderTargetView()
{
	com_ptr<ID3D11Texture2D> spTexture;
	m_spSwapChin->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&spTexture);
	if (spTexture)
		m_spDevice->CreateRenderTargetView(spTexture, nullptr, &m_spTargetView);

	return (nullptr != m_spTargetView);
}

bool LD3DDevice::createDepthStencilView()
{
	D3D11_TEXTURE2D_DESC desc;
	fillTexture2DDesc(desc);

	com_ptr<ID3D11Texture2D> spTexture;
	m_spDevice->CreateTexture2D(&desc, NULL, &spTexture);

	com_ptr<ID3D11DepthStencilView> spView;
	m_spDevice->CreateDepthStencilView(spTexture, nullptr, &spView);
	return (nullptr != spView);
}

void LD3DDevice::fillSwapChainDesc(HWND hwnd, DXGI_SWAP_CHAIN_DESC& chainDesc)
{
	DXGI_MODE_DESC& buff = chainDesc.BufferDesc;
	buff.Width = 640;
	buff.Height = 480;
	buff.RefreshRate.Numerator = 60;
	buff.RefreshRate.Denominator = 1;
	buff.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
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
	if (0 == quality)
	{
		HRESULT hr = m_spDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &quality);
		assert(quality > 0);
	}
	desc.Count = 1;
	desc.Quality = 0;
}

void LD3DDevice::fillTexture2DDesc(D3D11_TEXTURE2D_DESC& desc)
{
	desc.Width = 640;
	desc.Height = 480;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	fillSampleDesc(desc.SampleDesc);

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = 0;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
}
