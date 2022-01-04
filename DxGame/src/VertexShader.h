#pragma once
#include "Prerequisites.h"


class VertexShader
{
public:
	VertexShader(RenderSystem* p_system, const void* p_shader_bytecode, size_t shader_size);
	~VertexShader();
protected:
	ID3D11VertexShader* mp_vs;
	RenderSystem* mp_system;
	friend class RenderSystem;
	friend class DeviceContext;
};

