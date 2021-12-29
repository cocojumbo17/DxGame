#pragma once

class SwapChain;
class VertexBuffer;
class IndexBuffer;
class VertexShader;
class PixelShader;
class ConstantBuffer;

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* ctx);
	~DeviceContext();
	bool Release();
	void ClearRenderTargetColor(SwapChain* p_swap_chain, float r, float g, float b, float a);
	void SetVertexBuffer(VertexBuffer* p_vertex_buffer);
	void SetIndexBuffer(IndexBuffer* p_index_buffer);
	void SetConstantBufferVS(ConstantBuffer* p_constant_buffer);
	void SetConstantBufferPS(ConstantBuffer* p_constant_buffer);
	void SetVertexShader(VertexShader* p_vertex_shader);
	void SetPixelShader(PixelShader* p_pixel_shader);
	void DrawTriangleList(UINT vertex_number, UINT start_vertex_index);
	void DrawIndexedTriangleList(UINT index_number, UINT start_index_location, int base_index_shift);
	void DrawTriangleStrip(UINT vertex_number, UINT start_vertex_index);
	void SetViewport(UINT width, UINT height);
protected:
	ID3D11DeviceContext* mp_imm_ctx;
	friend class ConstantBuffer;
};

