#pragma once
#include "Prerequisites.h"

class PixelShader
{
public:
	PixelShader(RenderSystem* p_system, const void* p_shader_bytecode, size_t shader_size);
	~PixelShader();
protected:
	ID3D11PixelShader* mp_ps;
	RenderSystem* mp_system;
	friend class RenderSystem;
	friend class DeviceContext;
};

