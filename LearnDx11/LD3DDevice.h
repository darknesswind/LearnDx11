#pragma once
class LD3DDevice
{
public:
	LD3DDevice();
	~LD3DDevice();

	bool init(HWND hwnd);

protected:
	bool createDevice(DXGI_SWAP_CHAIN_DESC& chainDesc);
	bool createRenderTargetView();
	bool createDepthStencilView();

	void fillSwapChainDesc(HWND hwnd, DXGI_SWAP_CHAIN_DESC& desc);
	void fillSampleDesc(DXGI_SAMPLE_DESC& desc);
	void fillTexture2DDesc(D3D11_TEXTURE2D_DESC& desc);

private:
	com_ptr<IDXGISwapChain> m_spSwapChin;
	com_ptr<ID3D11Device> m_spDevice;
	com_ptr<ID3D11DeviceContext> m_spImmContext;
	com_ptr<ID3D11RenderTargetView> m_spTargetView;
};

