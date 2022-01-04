#include "pch.h"
#include "PixelShader.h"
#include "RenderSystem.h"
#include <exception>

PixelShader::PixelShader(RenderSystem* p_system, const void* p_shader_bytecode, size_t shader_size)
    : mp_ps(nullptr)
    , mp_system(p_system)
{
    HRESULT hr = mp_system->mp_d3d_device->CreatePixelShader(p_shader_bytecode, shader_size, nullptr, &mp_ps);
    if (FAILED(hr))
        throw(std::exception("PixelShader is not created properly."));
}

PixelShader::~PixelShader()
{
    SAFE_RELEASE(mp_ps);
}

