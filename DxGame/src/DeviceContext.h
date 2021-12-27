#pragma once

class SwapChain;
class VertexBuffer;
class VertexShader;

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* ctx);
	~DeviceContext();
	bool Release();
	void ClearRenderTargetColor(SwapChain* p_swap_chain, float r, float g, float b, float a);
	void SetVertexBuffer(VertexBuffer* p_vertex_buffer);
	void SetVertexShader(VertexShader* p_vertex_shader);
	void DrawTriangleList(UINT vertex_number, UINT start_vertex_index);
	void DrawTriangleStrip(UINT vertex_number, UINT start_vertex_index);
	void SetViewport(UINT width, UINT height);
protected:
	ID3D11DeviceContext* mp_imm_ctx;
};

