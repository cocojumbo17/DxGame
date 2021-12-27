#pragma once

class GraphicsEngine;
class DeviceContext;

class PixelShader
{
public:
	PixelShader();
	~PixelShader();
	void Release();
protected:
	bool Init(const void* p_shader_bytecode, size_t shader_size);

protected:
	ID3D11PixelShader* mp_ps;
	friend class GraphicsEngine;
	friend class DeviceContext;
};

