#pragma once

class SwapChain
{
public:
	SwapChain();
	virtual ~SwapChain();
	bool Init(HWND hwnd, int width, int height);
	bool Release();
	void Present(bool vsync);
protected:
	IDXGISwapChain* mp_swap_chain;
	ID3D11RenderTargetView* mp_rtv;
	friend class DeviceContext;
};

