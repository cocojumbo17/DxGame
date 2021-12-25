#include "pch.h"
#include "SwapChain.h"
#include "GraphicsEngine.h"


SwapChain::SwapChain()
    :mp_swap_chain(nullptr)
{
}

SwapChain::~SwapChain()
{
}

bool SwapChain::Init(HWND hwnd, int width, int height)
{
    DXGI_SWAP_CHAIN_DESC desc = { 0 };
    desc.BufferCount = 1;
    desc.BufferDesc.Width = width;
    desc.BufferDesc.Height = height;
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.OutputWindow = hwnd;
    desc.Windowed = TRUE;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    if (FAILED(GraphicsEngine::Instance()->mp_dx_factory->CreateSwapChain(GraphicsEngine::Instance()->mp_device, &desc, &mp_swap_chain)))
        return false;
    return true;
}

bool SwapChain::Release()
{
    SAFE_RELEASE(mp_swap_chain);
    delete this;
    return true;
}
