#pragma once
#include "Prerequisites.h"

class SwapChain
{
public:
	SwapChain(RenderSystem* p_system, HWND hwnd, int width, int height);
	virtual ~SwapChain();
	void Present(bool vsync);
	bool Resize(int width, int height);
	void SetFullScreen(bool is_fullscreen, int width, int height);
protected:
	void CreateTargetView();
	void CreateDepthStencil(int width, int height);
	void ResizeBuffers(int width, int height);
protected:
	IDXGISwapChain* mp_swap_chain;
	ID3D11RenderTargetView* mp_rtv;
	ID3D11DepthStencilView* mp_dsv;
	RenderSystem* mp_system;
	friend class DeviceContext;
};

