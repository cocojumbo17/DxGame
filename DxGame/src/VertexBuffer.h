#pragma once
#include "Prerequisites.h"


class VertexBuffer
{
public:
	VertexBuffer(RenderSystem* p_system, void* vertex_list, size_t vertex_size, size_t list_size, const void* p_shader_bytecode, size_t shader_size);
	~VertexBuffer();
	size_t GetListSize();
protected:
	size_t m_vertex_size;
	size_t m_list_size;
	ID3D11Buffer* mp_vertex_buffer;
	ID3D11InputLayout* mp_input_layout;
	RenderSystem* mp_system;
	friend class DeviceContext;
};

