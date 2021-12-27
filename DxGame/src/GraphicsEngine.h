#pragma once

class SwapChain;
class DeviceContext;
class VertexBuffer;

class GraphicsEngine
{
public:
	virtual ~GraphicsEngine();
	bool Init();
	bool Release();
	static GraphicsEngine* Instance();
	SwapChain* CreateSwapChain();
	VertexBuffer* CreateVertexBuffer();
	DeviceContext* GetDeviceContext();

	bool CreateShaders();
	void SetShaders();
	void GetShadersBytecodeAndSize(void **pp_shader_bytecode, unsigned int* p_shader_size);
private:
	GraphicsEngine();
protected:
	ID3D11Device* mp_d3d_device;
	D3D_FEATURE_LEVEL m_feature_level;
	IDXGIFactory* mp_dx_factory;
	DeviceContext* mp_ctx;
	ID3D11DeviceContext* mp_imm_ctx;
	ID3DBlob* mp_vs_blob;
	ID3DBlob* mp_ps_blob;
	ID3D11VertexShader* mp_vs;
	ID3D11PixelShader* mp_ps;
	friend class SwapChain;
	friend class VertexBuffer;
};

