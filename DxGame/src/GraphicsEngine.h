#pragma once

class SwapChain;
class DeviceContext;
class VertexBuffer;
class VertexShader;

class GraphicsEngine
{
public:
	virtual ~GraphicsEngine();
	bool Init();
	bool Release();
	static GraphicsEngine* Instance();
	SwapChain* CreateSwapChain();
	VertexBuffer* CreateVertexBuffer();
	
	bool CompileVertexShader(const wchar_t* shader_file, const char* entry_point_name, void** pp_shader_bytecode, size_t& shader_size);
	VertexShader* CreateVertexShader(const void* p_shader_bytecode, size_t shader_size);
	void ReleaseCompiledShader();


	DeviceContext* GetDeviceContext();

	bool CreateShaders();
	void SetShaders();
private:
	GraphicsEngine();
protected:
	ID3D11Device* mp_d3d_device;
	D3D_FEATURE_LEVEL m_feature_level;
	IDXGIFactory* mp_dx_factory;
	DeviceContext* mp_ctx;
	ID3D11DeviceContext* mp_imm_ctx;
	ID3DBlob* mp_blob;

	ID3DBlob* mp_ps_blob;
	ID3D11PixelShader* mp_ps;
	friend class SwapChain;
	friend class VertexBuffer;
	friend class VertexShader;
};

