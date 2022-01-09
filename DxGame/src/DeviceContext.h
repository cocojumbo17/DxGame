#pragma once
#include "Prerequisites.h"

class DeviceContext
{
public:
	DeviceContext(RenderSystem* p_system, ID3D11DeviceContext* ctx);
	~DeviceContext();
	void ClearRenderTargetColor(const SwapChainPtr& p_swap_chain, float r, float g, float b, float a);
	void SetVertexBuffer(const VertexBufferPtr& p_vertex_buffer);
	void SetIndexBuffer(const IndexBufferPtr& p_index_buffer);
	void SetConstantBufferVS(const ConstantBufferPtr& p_constant_buffer);
	void SetConstantBufferPS(const ConstantBufferPtr& p_constant_buffer);
	//void SetTextureVS(const TexturePtr* p_textures, size_t num_textures);
	void SetTexturePS(const TexturePtr* p_textures, size_t num_textures);
	void SetVertexShader(const VertexShaderPtr& p_vertex_shader);
	void SetPixelShader(const PixelShaderPtr& p_pixel_shader);
	void DrawTriangleList(UINT vertex_number, UINT start_vertex_index);
	void DrawIndexedTriangleList(UINT index_number, UINT start_index_location, int base_index_shift);
	void DrawTriangleStrip(UINT vertex_number, UINT start_vertex_index);
	void SetViewport(UINT width, UINT height);
protected:
	ID3D11DeviceContext* mp_imm_ctx;
	RenderSystem* mp_system;
	friend class ConstantBuffer;
};

