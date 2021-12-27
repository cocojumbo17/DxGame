#include "pch.h"
#include "PixelShader.h"
#include "GraphicsEngine.h"

PixelShader::PixelShader()
    :mp_ps(nullptr)
{
}

PixelShader::~PixelShader()
{
}

void PixelShader::Release()
{
    SAFE_RELEASE(mp_ps);
    delete this;
}

bool PixelShader::Init(const void* p_shader_bytecode, size_t shader_size)
{
    HRESULT hr = GraphicsEngine::Instance()->mp_d3d_device->CreatePixelShader(p_shader_bytecode, shader_size, nullptr, &mp_ps);
    if (SUCCEEDED(hr))
        return true;
    return false;
}
