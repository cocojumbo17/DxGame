#pragma once
#include "Prerequisites.h"

class SwapChain
{
public:
	SwapChain(RenderSystem* p_system, HWND hwnd, int width, int height);
	virtual ~SwapChain();
	void Present(bool vsync);
	bool Resize();
protected:
	void CreateTargetView();
	void CreateDepthStencil();
protected:
	IDXGISwapChain* mp_swap_chain;
	ID3D11RenderTargetView* mp_rtv;
	ID3D11DepthStencilView* mp_dsv;
	RenderSystem* mp_system;
	friend class DeviceContext;
};

