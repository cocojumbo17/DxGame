#pragma once


class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();
	bool Load(void* index_list, size_t list_size);
	bool Release();
	size_t GetListSize();
protected:
	size_t m_list_size;
	ID3D11Buffer* mp_buffer;
	friend class DeviceContext;
};

