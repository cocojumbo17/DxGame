#include "pch.h"
#include "VertexShader.h"
#include "GraphicsEngine.h"

VertexShader::VertexShader()
    :mp_vs(nullptr)
{
}

VertexShader::~VertexShader()
{
}

void VertexShader::Release()
{
    SAFE_RELEASE(mp_vs);
    delete this;
}

bool VertexShader::Init(const void* p_shader_bytecode, size_t shader_size)
{
    HRESULT hr = GraphicsEngine::Instance()->mp_d3d_device->CreateVertexShader(p_shader_bytecode, shader_size, nullptr, &mp_vs);
    if (SUCCEEDED(hr))
        return true;
    return false;
}
