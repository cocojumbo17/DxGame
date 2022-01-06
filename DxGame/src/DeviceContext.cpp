#include "pch.h"
#include "DeviceContext.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RenderSystem.h"
#include "Texture.h"

DeviceContext::DeviceContext(RenderSystem* p_system, ID3D11DeviceContext* ctx)
	: mp_imm_ctx(ctx)
	, mp_system(p_system)
{
}

DeviceContext::~DeviceContext()
{
	SAFE_RELEASE(mp_imm_ctx);
}

void DeviceContext::ClearRenderTargetColor(const SwapChainPtr& p_swap_chain, float r, float g, float b, float a)
{
	FLOAT color[] = { r,g,b,a };
	mp_imm_ctx->ClearRenderTargetView(p_swap_chain->mp_rtv, color);
	mp_imm_ctx->ClearDepthStencilView(p_swap_chain->mp_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	mp_imm_ctx->OMSetRenderTargets(1, &p_swap_chain->mp_rtv, p_swap_chain->mp_dsv);
}

void DeviceContext::SetVertexBuffer(const VertexBufferPtr& p_vertex_buffer)
{
	UINT strides[] = { (UINT)p_vertex_buffer->m_vertex_size };
	UINT offsets[] = { 0 };
	mp_imm_ctx->IASetVertexBuffers(0, 1, &p_vertex_buffer->mp_vertex_buffer, strides, offsets);
	mp_imm_ctx->IASetInputLayout(p_vertex_buffer->mp_input_layout);
}

void DeviceContext::SetIndexBuffer(const IndexBufferPtr& p_index_buffer)
{
	mp_imm_ctx->IASetIndexBuffer(p_index_buffer->mp_buffer, DXGI_FORMAT_R32_UINT, 0);
}

void DeviceContext::SetConstantBufferVS(const ConstantBufferPtr& p_constant_buffer)
{
	mp_imm_ctx->VSSetConstantBuffers(0, 1, &p_constant_buffer->mp_buffer);
}

void DeviceContext::SetConstantBufferPS(const ConstantBufferPtr& p_constant_buffer)
{
	mp_imm_ctx->PSSetConstantBuffers(0, 1, &p_constant_buffer->mp_buffer);
}

//void DeviceContext::SetTextureVS(const TexturePtr& p_texture)
//{
//	mp_imm_ctx->VSSetShaderResources(0, 1, &p_texture->mp_shader_resource_view);
//}

void DeviceContext::SetTexturePS(const TexturePtr& p_texture)
{
	mp_imm_ctx->PSSetShaderResources(0, 1, &p_texture->mp_shader_resource_view);
}

void DeviceContext::SetVertexShader(const VertexShaderPtr& p_vertex_shader)
{
	mp_imm_ctx->VSSetShader(p_vertex_shader->mp_vs, nullptr, 0);
}

void DeviceContext::SetPixelShader(const PixelShaderPtr& p_pixel_shader)
{
	mp_imm_ctx->PSSetShader(p_pixel_shader->mp_ps, nullptr, 0);
}

void DeviceContext::DrawTriangleList(UINT vertex_number, UINT start_vertex_index)
{
	mp_imm_ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mp_imm_ctx->Draw(vertex_number, start_vertex_index);
}

void DeviceContext::DrawIndexedTriangleList(UINT index_number, UINT start_index_location, int base_index_shift)
{
	mp_imm_ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mp_imm_ctx->DrawIndexed(index_number, start_index_location, base_index_shift);
}

void DeviceContext::DrawTriangleStrip(UINT vertex_number, UINT start_vertex_index)
{
	mp_imm_ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	mp_imm_ctx->Draw(vertex_number, start_vertex_index);
}

void DeviceContext::SetViewport(UINT width, UINT height)
{
	D3D11_VIEWPORT viewport = { 0 };
	viewport.Height = (FLOAT)height;
	viewport.Width = (FLOAT)width;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	mp_imm_ctx->RSSetViewports(1, &viewport);
}
