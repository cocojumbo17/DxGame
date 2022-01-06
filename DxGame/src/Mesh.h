#pragma once
#include "Resource.h"
#include "Prerequisites.h"

class Mesh:public Resource
{
public:
	Mesh(const wchar_t* full_path);
	virtual ~Mesh();
	const VertexBufferPtr& GetVertexBuffer();
	const IndexBufferPtr& GetIndexBuffer();

protected:
	VertexBufferPtr mp_vb;
	IndexBufferPtr mp_ib;
	friend class DeviceContext;
};

