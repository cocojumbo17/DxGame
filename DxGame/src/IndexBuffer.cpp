#include "pch.h"
#include "IndexBuffer.h"
#include "GraphicsEngine.h"


IndexBuffer::IndexBuffer()
: m_list_size(0)
, mp_buffer(nullptr)
{
}

IndexBuffer::~IndexBuffer()
{
}

bool IndexBuffer::Load(void* index_list, size_t list_size)
{
    SAFE_RELEASE(mp_buffer);


    D3D11_BUFFER_DESC buf_desc = { 0 };
    buf_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    buf_desc.ByteWidth = (UINT)(4 * list_size);
    buf_desc.CPUAccessFlags = 0;
    buf_desc.MiscFlags = 0;
    buf_desc.Usage = D3D11_USAGE_DEFAULT;

    m_list_size = list_size;

    D3D11_SUBRESOURCE_DATA sub_data = { 0 };
    sub_data.pSysMem = index_list;

    HRESULT hr = GraphicsEngine::Instance()->mp_d3d_device->CreateBuffer(&buf_desc, &sub_data, &mp_buffer);
    if (FAILED(hr))
        return false;

    return true;
}

bool IndexBuffer::Release()
{
    SAFE_RELEASE(mp_buffer);
    delete this;
    return true;
}

size_t IndexBuffer::GetListSize()
{
    return m_list_size;
}
