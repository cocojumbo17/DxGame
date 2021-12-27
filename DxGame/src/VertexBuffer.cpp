#include "pch.h"
#include "VertexBuffer.h"
#include "GraphicsEngine.h"


VertexBuffer::VertexBuffer()
: m_vertex_size(0)
, m_list_size(0)
, mp_input_layout(nullptr)
, mp_vertex_buffer(nullptr)
{
}

VertexBuffer::~VertexBuffer()
{
}

bool VertexBuffer::Load(void* vertex_list, unsigned int vertex_size, unsigned int list_size, const void* p_shader_bytecode, int shader_size)
{

    SAFE_RELEASE(mp_input_layout);
    SAFE_RELEASE(mp_vertex_buffer);


    D3D11_BUFFER_DESC buf_desc = { 0 };
    buf_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buf_desc.ByteWidth = vertex_size * list_size;
    buf_desc.CPUAccessFlags = 0;
    buf_desc.MiscFlags = 0;
    buf_desc.Usage = D3D11_USAGE_DEFAULT;
    //buf_desc.StructureByteStride = vertex_size;
    m_vertex_size = vertex_size;
    m_list_size = list_size;

    D3D11_SUBRESOURCE_DATA sub_data = { 0 };
    sub_data.pSysMem = vertex_list;

    HRESULT hr = GraphicsEngine::Instance()->mp_d3d_device->CreateBuffer(&buf_desc, &sub_data, &mp_vertex_buffer);
    if (FAILED(hr))
        return false;

    D3D11_INPUT_ELEMENT_DESC element_descs[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    hr = GraphicsEngine::Instance()->mp_d3d_device->CreateInputLayout(element_descs, ARRAYSIZE(element_descs), p_shader_bytecode, shader_size, &mp_input_layout);
    if (FAILED(hr))
        return false;

    return true;
}

bool VertexBuffer::Release()
{
    SAFE_RELEASE(mp_input_layout);
    SAFE_RELEASE(mp_vertex_buffer);
    delete this;
    return true;
}

unsigned int VertexBuffer::GetListSize()
{
    return m_list_size;
}
