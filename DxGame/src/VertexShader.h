#pragma once

class GraphicsEngine;
class DeviceContext;

class VertexShader
{
public:
	VertexShader();
	~VertexShader();
	void Release();
protected:
	bool Init(const void* p_shader_bytecode, size_t shader_size);

protected:
	ID3D11VertexShader* mp_vs;
	friend class GraphicsEngine;
	friend class DeviceContext;
};

