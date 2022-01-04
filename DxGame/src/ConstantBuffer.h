#pragma once
#include "Prerequisites.h"

class ConstantBuffer
{
public:
	ConstantBuffer(RenderSystem* p_system, void* p_buffer, size_t buffer_size);
	~ConstantBuffer();
	void Update(DeviceContext* p_ctx, void* p_buffer);
protected:
	ID3D11Buffer* mp_buffer;
	RenderSystem* mp_system;
	friend class DeviceContext;
};

