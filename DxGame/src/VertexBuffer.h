#pragma once


class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();
	bool Load(void* vertex_list, unsigned int vertex_size, unsigned int list_size, const void* p_shader_bytecode, int shader_size);
	bool Release();
	unsigned int GetListSize();
protected:
	unsigned int m_vertex_size;
	unsigned int m_list_size;
	ID3D11Buffer* mp_vertex_buffer;
	ID3D11InputLayout* mp_input_layout;
	friend class DeviceContext;
};

