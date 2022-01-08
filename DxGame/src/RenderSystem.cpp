#include "pch.h"
#include "RenderSystem.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include <d3dcompiler.h>


void RenderSystem::InitRasterizerState()
{
	D3D11_RASTERIZER_DESC desc = { 0 };
	desc.CullMode = D3D11_CULL_FRONT;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.DepthClipEnable = true;

	HRESULT hr = mp_d3d_device->CreateRasterizerState(&desc, &mp_front_rasterizer_state);

	desc.CullMode = D3D11_CULL_BACK;
	hr = mp_d3d_device->CreateRasterizerState(&desc, &mp_back_rasterizer_state);
}

void RenderSystem::SetRasterizerState(bool is_cull_front)
{
	if (is_cull_front)
		mp_imm_ctx->RSSetState(mp_front_rasterizer_state);
	else
		mp_imm_ctx->RSSetState(mp_back_rasterizer_state);
}


RenderSystem::RenderSystem()
	: mp_d3d_device(nullptr)
	, mp_dx_factory(nullptr)
	, mp_blob(nullptr)
	, mp_front_rasterizer_state(nullptr)
	, mp_back_rasterizer_state(nullptr)
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
		throw(std::exception("RenderSystem is not created properly."));

	mp_ctx = std::make_shared<DeviceContext>(this, mp_imm_ctx);

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
	InitRasterizerState();
}

RenderSystem::~RenderSystem()
{
	SAFE_RELEASE(mp_front_rasterizer_state);
	SAFE_RELEASE(mp_back_rasterizer_state);
	SAFE_RELEASE(mp_dx_factory);
	SAFE_RELEASE(mp_d3d_device);
}

SwapChainPtr RenderSystem::CreateSwapChain(HWND hwnd, int width, int height)
{
	SwapChainPtr p_sch = nullptr;
	try
	{
		p_sch = std::make_shared<SwapChain>(this, hwnd, width, height);
	}
	catch (const std::exception& e)
	{
		Logger::PrintLog("[ERROR] %s", e.what());
	}
	return p_sch;
}

VertexBufferPtr RenderSystem::CreateVertexBuffer(void* vertex_list, size_t vertex_size, size_t list_size, const void* p_shader_bytecode, size_t shader_size)
{
	VertexBufferPtr p_vb = nullptr;
	try
	{
		p_vb = std::make_shared<VertexBuffer>(this, vertex_list, vertex_size, list_size, p_shader_bytecode, shader_size);
	}
	catch (const std::exception& e)
	{
		Logger::PrintLog("[ERROR] %s", e.what());
	}
	return p_vb;
}

IndexBufferPtr RenderSystem::CreateIndexBuffer(void* index_list, size_t list_size)
{
	IndexBufferPtr p_ib = nullptr;
	try
	{
		p_ib = std::make_shared<IndexBuffer>(this, index_list, list_size);
	}
	catch (const std::exception& e)
	{
		Logger::PrintLog("[ERROR] %s", e.what());
	}
	return p_ib;
}

ConstantBufferPtr RenderSystem::CreateConstantBuffer(void* p_buffer, size_t buffer_size)
{
	ConstantBufferPtr p_cb = nullptr;
	try
	{
		p_cb = std::make_shared<ConstantBuffer>(this, p_buffer, buffer_size);
	}
	catch (const std::exception& e)
	{
		Logger::PrintLog("[ERROR] %s", e.what());
	}
	return p_cb;
}

VertexShaderPtr RenderSystem::CreateVertexShader(const void* p_shader_bytecode, size_t shader_size)
{
	VertexShaderPtr p_vs = nullptr;
	try
	{
		p_vs = std::make_shared<VertexShader>(this, p_shader_bytecode, shader_size);
	}
	catch (const std::exception& e)
	{
		Logger::PrintLog("[ERROR] %s", e.what());
	}
	return p_vs;
}

PixelShaderPtr RenderSystem::CreatePixelShader(const void* p_shader_bytecode, size_t shader_size)
{
	PixelShaderPtr p_ps = nullptr;
	try
	{
		p_ps = std::make_shared<PixelShader>(this, p_shader_bytecode, shader_size);
	}
	catch (const std::exception& e)
	{
		Logger::PrintLog("[ERROR] %s", e.what());
	}
	return p_ps;
}

bool RenderSystem::CompileVertexShader(const wchar_t* shader_file, const char* entry_point_name, void** pp_shader_bytecode, size_t& shader_size)
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
		char* err = (char*)errblob->GetBufferPointer();
		Logger::PrintLog("[ERROR] CompilePixelShader: %s", err);
		SAFE_RELEASE(errblob);
	}
	return false;
}

bool RenderSystem::CompilePixelShader(const wchar_t* shader_file, const char* entry_point_name, void** pp_shader_bytecode, size_t& shader_size)
{
	ID3DBlob* errblob;
	HRESULT hr = D3DCompileFromFile(shader_file, nullptr, nullptr, entry_point_name, "ps_5_0", 0, 0, &mp_blob, &errblob);
	if (SUCCEEDED(hr))
	{
		*pp_shader_bytecode = mp_blob->GetBufferPointer();
		shader_size = mp_blob->GetBufferSize();
		return true;
	}
	else
	{
		char* err = (char*)errblob->GetBufferPointer();
		Logger::PrintLog("[ERROR] CompilePixelShader: %s", err);
		SAFE_RELEASE(errblob);
	}
	return false;
}


void RenderSystem::ReleaseCompiledShader()
{
	SAFE_RELEASE(mp_blob);
}

DeviceContextPtr RenderSystem::GetDeviceContext()
{
	return mp_ctx;
}

