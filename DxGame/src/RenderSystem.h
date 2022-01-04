#pragma once
#include "Prerequisites.h"

class RenderSystem
{
public:
	virtual ~RenderSystem();
	bool Init();
	bool Release();
	SwapChain* CreateSwapChain(HWND hwnd, int width, int height);
	VertexBuffer* CreateVertexBuffer(void* vertex_list, size_t vertex_size, size_t list_size, const void* p_shader_bytecode, size_t shader_size);
	IndexBuffer* CreateIndexBuffer(void* index_list, size_t list_size);
	ConstantBuffer* CreateConstantBuffer(void* p_buffer, size_t buffer_size);

	bool CompileVertexShader(const wchar_t* shader_file, const char* entry_point_name, void** pp_shader_bytecode, size_t& shader_size);
	VertexShader* CreateVertexShader(const void* p_shader_bytecode, size_t shader_size);

	bool CompilePixelShader(const wchar_t* shader_file, const char* entry_point_name, void** pp_shader_bytecode, size_t& shader_size);
	PixelShader* CreatePixelShader(const void* p_shader_bytecode, size_t shader_size);

	void ReleaseCompiledShader();


	DeviceContext* GetDeviceContext();

private:
	RenderSystem();
protected:
	ID3D11Device* mp_d3d_device;
	D3D_FEATURE_LEVEL m_feature_level;
	IDXGIFactory* mp_dx_factory;
	DeviceContext* mp_ctx;
	ID3D11DeviceContext* mp_imm_ctx;

	ID3DBlob* mp_blob;

	friend class SwapChain;
	friend class VertexBuffer;
	friend class IndexBuffer;
	friend class VertexShader;
	friend class PixelShader;
	friend class ConstantBuffer;
	friend class GraphicsEngine;
};

