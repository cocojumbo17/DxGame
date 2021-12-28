#pragma once


class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();
	bool Load(void* vertex_list, size_t vertex_size, size_t list_size, const void* p_shader_bytecode, size_t shader_size);
	bool Release();
	size_t GetListSize();
protected:
	size_t m_vertex_size;
	size_t m_list_size;
	ID3D11Buffer* mp_vertex_buffer;
	ID3D11InputLayout* mp_input_layout;
	friend class DeviceContext;
};

