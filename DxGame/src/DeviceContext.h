#pragma once
#include "Prerequisites.h"

class DeviceContext
{
public:
	DeviceContext(RenderSystem* p_system, ID3D11DeviceContext* ctx);
	~DeviceContext();
	void ClearRenderTargetColor(SwapChainPtr p_swap_chain, float r, float g, float b, float a);
	void SetVertexBuffer(VertexBufferPtr p_vertex_buffer);
	void SetIndexBuffer(IndexBufferPtr p_index_buffer);
	void SetConstantBufferVS(ConstantBufferPtr p_constant_buffer);
	void SetConstantBufferPS(ConstantBufferPtr p_constant_buffer);
	void SetVertexShader(VertexShaderPtr p_vertex_shader);
	void SetPixelShader(PixelShaderPtr p_pixel_shader);
	void DrawTriangleList(UINT vertex_number, UINT start_vertex_index);
	void DrawIndexedTriangleList(UINT index_number, UINT start_index_location, int base_index_shift);
	void DrawTriangleStrip(UINT vertex_number, UINT start_vertex_index);
	void SetViewport(UINT width, UINT height);
protected:
	ID3D11DeviceContext* mp_imm_ctx;
	RenderSystem* mp_system;
	friend class ConstantBuffer;
};

