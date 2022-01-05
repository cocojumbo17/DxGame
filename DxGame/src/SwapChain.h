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
	IDXGISwapChain* mp_swap_chain;
	ID3D11RenderTargetView* mp_rtv;
	RenderSystem* mp_system;
	friend class DeviceContext;
};

