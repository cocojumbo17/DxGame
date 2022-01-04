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


RenderSystem::RenderSystem()
	: mp_d3d_device(nullptr)
	, mp_ctx(nullptr)
	, mp_dx_factory(nullptr)
	, mp_blob(nullptr)
{
}

RenderSystem::~RenderSystem()
{
}

bool RenderSystem::Init()
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

	mp_ctx = new DeviceContext(this, mp_imm_ctx);

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

bool RenderSystem::Release()
{
	SAFE_RELEASE(mp_dx_factory);
	delete mp_ctx;
	SAFE_RELEASE(mp_d3d_device);
	return true;
}

SwapChain* RenderSystem::CreateSwapChain(HWND hwnd, int width, int height)
{
	SwapChain* p_sch = nullptr;
	try
	{
		p_sch = new SwapChain(this, hwnd, width, height);
	}
	catch (const std::exception& e)
	{
		Logger::PrintLog("[ERROR] %s", e.what());
	}
	return p_sch;
}

VertexBuffer* RenderSystem::CreateVertexBuffer(void* vertex_list, size_t vertex_size, size_t list_size, const void* p_shader_bytecode, size_t shader_size)
{
	VertexBuffer* p_vb = nullptr;
	try
	{
		p_vb = new VertexBuffer(this, vertex_list, vertex_size, list_size, p_shader_bytecode, shader_size);
	}
	catch (const std::exception& e)
	{
		Logger::PrintLog("[ERROR] %s", e.what());
	}
	return p_vb;
}

IndexBuffer* RenderSystem::CreateIndexBuffer(void* index_list, size_t list_size)
{
	IndexBuffer* p_ib = nullptr;
	try
	{
		p_ib = new IndexBuffer(this, index_list, list_size);
	}
	catch (const std::exception& e)
	{
		Logger::PrintLog("[ERROR] %s", e.what());
	}
	return p_ib;
}

ConstantBuffer* RenderSystem::CreateConstantBuffer(void* p_buffer, size_t buffer_size)
{
	ConstantBuffer* p_cb = nullptr;
	try
	{
		p_cb = new ConstantBuffer(this, p_buffer, buffer_size);
	}
	catch (const std::exception& e)
	{
		Logger::PrintLog("[ERROR] %s", e.what());
	}
	return p_cb;
}

VertexShader* RenderSystem::CreateVertexShader(const void* p_shader_bytecode, size_t shader_size)
{
	VertexShader* p_vs = nullptr;
	try
	{
		p_vs = new VertexShader(this, p_shader_bytecode, shader_size);
	}
	catch (const std::exception& e)
	{
		Logger::PrintLog("[ERROR] %s", e.what());
	}
	return p_vs;
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
		size_t len = errblob->GetBufferSize();
		char* err = (char*)errblob->GetBufferPointer();
		wchar_t* w_err = new wchar_t[len];
		mbstowcs_s(nullptr, w_err, len, err, len);
		Logger::PrintLog("[ERROR] CompilePixelShader: %s", w_err);
		delete[]w_err;
		SAFE_RELEASE(errblob);
	}
	return false;
}

PixelShader* RenderSystem::CreatePixelShader(const void* p_shader_bytecode, size_t shader_size)
{
	PixelShader* p_ps = nullptr;
	try
	{
		p_ps = new PixelShader(this, p_shader_bytecode, shader_size);
	}
	catch (const std::exception& e)
	{
		Logger::PrintLog("[ERROR] %s", e.what());
	}
	return p_ps;
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
		size_t len = errblob->GetBufferSize();
		char* err = (char*)errblob->GetBufferPointer();
		wchar_t* w_err = new wchar_t[len];
		mbstowcs_s(nullptr, w_err, len, err, len);
		Logger::PrintLog("[ERROR] CompilePixelShader: %s", w_err);
		delete[]w_err;
		SAFE_RELEASE(errblob);
	}
	return false;
}


void RenderSystem::ReleaseCompiledShader()
{
	SAFE_RELEASE(mp_blob);
}

DeviceContext* RenderSystem::GetDeviceContext()
{
	return mp_ctx;
}

