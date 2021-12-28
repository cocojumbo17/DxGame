#pragma once

class DeviceContext;

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();
	bool Load(void* p_buffer, size_t buffer_size);
	void Update(DeviceContext* p_ctx, void* p_buffer);
	bool Release();
protected:
	ID3D11Buffer* mp_buffer;
	friend class DeviceContext;
};

