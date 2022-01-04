#include "pch.h"
#include "VertexShader.h"
#include "RenderSystem.h"
#include <exception>

VertexShader::VertexShader(RenderSystem* p_system, const void* p_shader_bytecode, size_t shader_size)
    : mp_vs(nullptr)
    , mp_system(p_system)
{
    HRESULT hr = mp_system->mp_d3d_device->CreateVertexShader(p_shader_bytecode, shader_size, nullptr, &mp_vs);
    if (FAILED(hr))
        throw(std::exception("VertexShader is not created properly."));
}

VertexShader::~VertexShader()
{
    SAFE_RELEASE(mp_vs);
}

