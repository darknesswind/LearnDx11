#pragma once
class LD3DApplication;
class LMainWindow;
class LD3DDevice
{
public:
	LD3DDevice(LD3DApplication* pApp);
	~LD3DDevice();

	bool setup();
	void swap();

	bool onResize();

	ID3D11Device* device() const { return m_spDevice; }
	ID3D11DeviceContext* immContext() const { return m_spImmContext; }
	IDXGISwapChain* swapChain() const { return m_spSwapChin; }

protected:
	bool createDevice();
	bool createSwapChain();
	bool createRenderTargetView();
	bool createDepthStencilView();

	void fillSwapChainDesc(HWND hwnd, DXGI_SWAP_CHAIN_DESC& desc);
	void fillSampleDesc(DXGI_SAMPLE_DESC& desc);
	void fillDepthTexture2DDesc(D3D11_TEXTURE2D_DESC& desc);

	void printDeviceInfo();
	static HRESULT getFactory(ID3D11Device* pDevice, IDXGIFactory** ppFactory);
private:
	LD3DApplication* m_pApp;
	LMainWindow* m_pWindow;
	com_ptr<IDXGIFactory> m_spFactory;
	com_ptr<ID3D11Device> m_spDevice;
	com_ptr<ID3D11DeviceContext> m_spImmContext;
	com_ptr<IDXGISwapChain> m_spSwapChin;
	com_ptr<ID3D11RenderTargetView> m_spTargetView;
	com_ptr<ID3D11DepthStencilView> m_spDepthView;

	bool m_bUse4xMSAA;

	DXGI_FORMAT	m_clrFormat;
};

