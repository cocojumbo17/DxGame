#include "pch.h"
#include "SwapChain.h"
#include "GraphicsEngine.h"


SwapChain::SwapChain()
    : mp_swap_chain(nullptr)
    , mp_rtv(nullptr)
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

    HRESULT hr = GraphicsEngine::Instance()->mp_dx_factory->CreateSwapChain(GraphicsEngine::Instance()->mp_d3d_device, &desc, &mp_swap_chain);
    if (FAILED(hr))
        return false;

    ID3D11Texture2D* p_buf = nullptr;
    hr = mp_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&p_buf);
    if (FAILED(hr))
        return false;
    hr = GraphicsEngine::Instance()->mp_d3d_device->CreateRenderTargetView((ID3D11Resource*)p_buf, nullptr, &mp_rtv);
    SAFE_RELEASE(p_buf);
    if (FAILED(hr))
        return false;

    return true;
}

bool SwapChain::Release()
{
    SAFE_RELEASE(mp_rtv);
    SAFE_RELEASE(mp_swap_chain);
    delete this;
    return true;
}

void SwapChain::Present(bool vsync)
{
    mp_swap_chain->Present(vsync, NULL);
}
