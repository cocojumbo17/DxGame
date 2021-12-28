#include "pch.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include <d3dcompiler.h>


GraphicsEngine::GraphicsEngine()
	: mp_d3d_device(nullptr)
	, mp_ctx(nullptr)
	, mp_dx_factory(nullptr)
	, mp_blob(nullptr)
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
		res = D3D11CreateDevice(nullptr, driver_types[driver_index], NULL, 0, feature_levels, ARRAYSIZE(feature_levels), D3D11_SDK_VERSION, &mp_d3d_device, &m_feature_level, &mp_imm_ctx);
		if (SUCCEEDED(res))
			break;
	}
	if (FAILED(res))
		return false;

	mp_ctx = new DeviceContext(mp_imm_ctx);

	IDXGIDevice* p_device = nullptr;
	IDXGIAdapter* p_adapter = nullptr;
	
	if (SUCCEEDED(mp_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&p_device)))
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
	SAFE_RELEASE(mp_dx_factory);
	SAFE_RELEASE(mp_ctx);
	SAFE_RELEASE(mp_d3d_device);
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

VertexBuffer* GraphicsEngine::CreateVertexBuffer()
{
	return new VertexBuffer();
}

ConstantBuffer* GraphicsEngine::CreateConstantBuffer()
{
	return new ConstantBuffer();
}

VertexShader* GraphicsEngine::CreateVertexShader(const void* p_shader_bytecode, size_t shader_size)
{
	VertexShader* p_vs = new VertexShader();
	if (!p_vs->Init(p_shader_bytecode, shader_size))
	{
		delete p_vs;
		return nullptr;
	}
	return p_vs;
}

bool GraphicsEngine::CompileVertexShader(const wchar_t* shader_file, const char* entry_point_name, void** pp_shader_bytecode, size_t& shader_size)
{
	ID3DBlob* errblob;
	HRESULT hr = D3DCompileFromFile(shader_file, nullptr, nullptr, entry_point_name, "vs_5_0", 0, 0, &mp_blob, &errblob);
	if (SUCCEEDED(hr))
	{
		*pp_shader_bytecode = mp_blob->GetBufferPointer();
		shader_size = mp_blob->GetBufferSize();
		return true;
	}
	else
	{
		void* err = errblob->GetBufferPointer();
		SAFE_RELEASE(errblob);
	}
	return false;
}

PixelShader* GraphicsEngine::CreatePixelShader(const void* p_shader_bytecode, size_t shader_size)
{
	PixelShader* p_ps = new PixelShader();
	if (!p_ps->Init(p_shader_bytecode, shader_size))
	{
		delete p_ps;
		return nullptr;
	}
	return p_ps;
}

bool GraphicsEngine::CompilePixelShader(const wchar_t* shader_file, const char* entry_point_name, void** pp_shader_bytecode, size_t& shader_size)
{
	HRESULT hr = D3DCompileFromFile(shader_file, nullptr, nullptr, entry_point_name, "ps_5_0", 0, 0, &mp_blob, nullptr);
	if (SUCCEEDED(hr))
	{
		*pp_shader_bytecode = mp_blob->GetBufferPointer();
		shader_size = mp_blob->GetBufferSize();
		return true;
	}
	return false;
}


void GraphicsEngine::ReleaseCompiledShader()
{
	SAFE_RELEASE(mp_blob);
}

DeviceContext* GraphicsEngine::GetDeviceContext()
{
	return mp_ctx;
}

