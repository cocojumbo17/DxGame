#include "pch.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"



GraphicsEngine::GraphicsEngine()
	: mp_device(nullptr)
	, mp_imm_ctx(nullptr)
	, mp_dx_factory(nullptr)
{
}

GraphicsEngine::~GraphicsEngine()
{
}

bool GraphicsEngine::Init()
{
	D3D_DRIVER_TYPE driver_types[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	
	D3D_FEATURE_LEVEL feature_levels[] = {
		D3D_FEATURE_LEVEL_11_0
	};
	HRESULT res = 0;
	for (int driver_index = 0; driver_index < ARRAYSIZE(driver_types); ++driver_index)
	{
		res = D3D11CreateDevice(nullptr, driver_types[driver_index], NULL, 0, feature_levels, ARRAYSIZE(feature_levels), D3D11_SDK_VERSION, &mp_device, &m_feature_level, &mp_imm_ctx);
		if (SUCCEEDED(res))
			break;
	}
	if (FAILED(res))
		return false;

	IDXGIDevice* p_device = nullptr;
	IDXGIAdapter* p_adapter = nullptr;
	
	if (SUCCEEDED(mp_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&p_device)))
	{
		if (SUCCEEDED(p_device->GetParent(__uuidof(IDXGIAdapter), (void**)&p_adapter)))
		{
			p_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&mp_dx_factory);
			SAFE_RELEASE(p_adapter);
		}
		SAFE_RELEASE(p_device);
	}
	return true;
}

bool GraphicsEngine::Release()
{
	SAFE_RELEASE(mp_dx_factory)
	SAFE_RELEASE(mp_imm_ctx);
	SAFE_RELEASE(mp_device);
	return true;
}

GraphicsEngine* GraphicsEngine::Instance()
{
	static GraphicsEngine inst;
	return &inst;
}

SwapChain* GraphicsEngine::CreateSwapChain()
{
	return new SwapChain();
}

