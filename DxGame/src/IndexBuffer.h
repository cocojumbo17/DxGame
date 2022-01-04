#pragma once
#include "Prerequisites.h"


class IndexBuffer
{
public:
	IndexBuffer(RenderSystem* p_system, void* index_list, size_t list_size);
	~IndexBuffer();
	size_t GetListSize();
protected:
	size_t m_list_size;
	ID3D11Buffer* mp_buffer;
	RenderSystem* mp_system;
	friend class DeviceContext;
};

