#include "pch.h"
#include "IndexBuffer.h"
#include "RenderSystem.h"
#include <exception>


IndexBuffer::IndexBuffer(RenderSystem* p_system, void* index_list, size_t list_size)
: m_list_size(0)
, mp_buffer(nullptr)
, mp_system(p_system)
{
    D3D11_BUFFER_DESC buf_desc = { 0 };
    buf_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    buf_desc.ByteWidth = (UINT)(4 * list_size);
    buf_desc.CPUAccessFlags = 0;
    buf_desc.MiscFlags = 0;
    buf_desc.Usage = D3D11_USAGE_DEFAULT;

    m_list_size = list_size;

    D3D11_SUBRESOURCE_DATA sub_data = { 0 };
    sub_data.pSysMem = index_list;

    HRESULT hr = mp_system->mp_d3d_device->CreateBuffer(&buf_desc, &sub_data, &mp_buffer);
    if (FAILED(hr))
        throw(std::exception("IndexBuffer is not created properly."));
}

IndexBuffer::~IndexBuffer()
{
    SAFE_RELEASE(mp_buffer);
}

size_t IndexBuffer::GetListSize()
{
    return m_list_size;
}
