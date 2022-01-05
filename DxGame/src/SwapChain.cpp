#include "pch.h"
#include "SwapChain.h"
#include "RenderSystem.h"
#include <exception>


SwapChain::SwapChain(RenderSystem* p_system, HWND hwnd, int width, int height)
    : mp_swap_chain(nullptr)
    , mp_rtv(nullptr)
    , mp_system(p_system)
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

    HRESULT hr = mp_system->mp_dx_factory->CreateSwapChain(mp_system->mp_d3d_device, &desc, &mp_swap_chain);
    if (FAILED(hr))
        throw(std::exception("CreateSwapChain is failed."));

    ID3D11Texture2D* p_buf = nullptr;
    hr = mp_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&p_buf);
    if (FAILED(hr))
        throw(std::exception("GetBuffer is failed."));
    hr = mp_system->mp_d3d_device->CreateRenderTargetView((ID3D11Resource*)p_buf, nullptr, &mp_rtv);
    SAFE_RELEASE(p_buf);
    if (FAILED(hr))
        throw(std::exception("CreateRenderTargetView is failed."));
}

SwapChain::~SwapChain()
{
    SAFE_RELEASE(mp_rtv);
    SAFE_RELEASE(mp_swap_chain);
}


void SwapChain::Present(bool vsync)
{
    mp_swap_chain->Present(vsync, NULL);
}

bool SwapChain::Resize()
{
    SAFE_RELEASE(mp_rtv);
    HRESULT hr = mp_swap_chain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
    if (SUCCEEDED(hr))
    {
        ID3D11Texture2D* p_buf = nullptr;
        hr = mp_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&p_buf);
        if (SUCCEEDED(hr))
        {
            hr = mp_system->mp_d3d_device->CreateRenderTargetView((ID3D11Resource*)p_buf, nullptr, &mp_rtv);
            SAFE_RELEASE(p_buf);
        }
    }
    return SUCCEEDED(hr);
}
