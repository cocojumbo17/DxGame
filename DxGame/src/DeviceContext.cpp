#include "pch.h"
#include "DeviceContext.h"
#include "SwapChain.h"
#include "VertexBuffer.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* ctx)
	: mp_imm_ctx(ctx)
{
}

DeviceContext::~DeviceContext()
{
}


bool DeviceContext::Release()
{
	SAFE_RELEASE(mp_imm_ctx);
	delete this;
	return true;
}

void DeviceContext::ClearRenderTargetColor(SwapChain* p_swap_chain, float r, float g, float b, float a)
{
	FLOAT color[] = { r,g,b,a };
	mp_imm_ctx->ClearRenderTargetView(p_swap_chain->mp_rtv, color);
	mp_imm_ctx->OMSetRenderTargets(1, &p_swap_chain->mp_rtv, nullptr);
}

void DeviceContext::SetVertexBuffer(VertexBuffer* p_vertex_buffer)
{
	UINT strides[] = { p_vertex_buffer->m_vertex_size };
	UINT offsets[] = { 0 };
	mp_imm_ctx->IASetVertexBuffers(0, 1, &p_vertex_buffer->mp_vertex_buffer, strides, offsets);
	mp_imm_ctx->IASetInputLayout(p_vertex_buffer->mp_input_layout);
}

void DeviceContext::DrawTriangleList(UINT vertex_number, UINT start_vertex_index)
{
	mp_imm_ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mp_imm_ctx->Draw(vertex_number, start_vertex_index);
}

void DeviceContext::DrawTriangleStrip(UINT vertex_number, UINT start_vertex_index)
{
	mp_imm_ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	mp_imm_ctx->Draw(vertex_number, start_vertex_index);
}

void DeviceContext::SetViewport(UINT width, UINT height)
{
	D3D11_VIEWPORT viewport = { 0 };
	viewport.Height = height;
	viewport.Width = width;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	mp_imm_ctx->RSSetViewports(1, &viewport);
}
