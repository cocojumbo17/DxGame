#include "pch.h"
#include "ConstantBuffer.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"

ConstantBuffer::ConstantBuffer()
: mp_buffer(nullptr)
{
}

ConstantBuffer::~ConstantBuffer()
{
}

bool ConstantBuffer::Load(void* p_buffer, size_t buffer_size)
{
    SAFE_RELEASE(mp_buffer);

    D3D11_BUFFER_DESC buf_desc = { 0 };
    buf_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buf_desc.ByteWidth = (UINT)buffer_size;
    buf_desc.CPUAccessFlags = 0;
    buf_desc.MiscFlags = 0;
    buf_desc.Usage = D3D11_USAGE_DEFAULT;
    
    D3D11_SUBRESOURCE_DATA sub_data = { 0 };
    sub_data.pSysMem = p_buffer;

    HRESULT hr = GraphicsEngine::Instance()->mp_d3d_device->CreateBuffer(&buf_desc, &sub_data, &mp_buffer);
    if (FAILED(hr))
        return false;

    return true;
}

void ConstantBuffer::Update(DeviceContext* p_ctx, void* p_buffer)
{
    p_ctx->mp_imm_ctx->UpdateSubresource(mp_buffer, 0, nullptr, p_buffer, 0, 0);
}

bool ConstantBuffer::Release()
{
    SAFE_RELEASE(mp_buffer);
    delete this;
    return true;
}
