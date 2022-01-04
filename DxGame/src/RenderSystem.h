#pragma once
#include "Prerequisites.h"

class RenderSystem
{
public:
	virtual ~RenderSystem();
	SwapChainPtr CreateSwapChain(HWND hwnd, int width, int height);
	VertexBufferPtr CreateVertexBuffer(void* vertex_list, size_t vertex_size, size_t list_size, const void* p_shader_bytecode, size_t shader_size);
	IndexBufferPtr CreateIndexBuffer(void* index_list, size_t list_size);
	ConstantBufferPtr CreateConstantBuffer(void* p_buffer, size_t buffer_size);
	VertexShaderPtr CreateVertexShader(const void* p_shader_bytecode, size_t shader_size);
	PixelShaderPtr CreatePixelShader(const void* p_shader_bytecode, size_t shader_size);

	bool CompileVertexShader(const wchar_t* shader_file, const char* entry_point_name, void** pp_shader_bytecode, size_t& shader_size);
	bool CompilePixelShader(const wchar_t* shader_file, const char* entry_point_name, void** pp_shader_bytecode, size_t& shader_size);
	void ReleaseCompiledShader();


	DeviceContextPtr GetDeviceContext();

private:
	RenderSystem();
protected:
	DeviceContextPtr mp_ctx;

	ID3D11Device* mp_d3d_device;
	D3D_FEATURE_LEVEL m_feature_level;
	IDXGIFactory* mp_dx_factory;
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

