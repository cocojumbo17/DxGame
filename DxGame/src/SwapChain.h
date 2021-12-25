#pragma once
class SwapChain
{
public:
	SwapChain();
	virtual ~SwapChain();
	bool Init(HWND hwnd, int width, int height);
	bool Release();
protected:
	IDXGISwapChain* mp_swap_chain;
};

