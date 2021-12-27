#pragma once

class SwapChain;
class DeviceContext;
class VertexBuffer;
class VertexShader;
class PixelShader;

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

	bool CompilePixelShader(const wchar_t* shader_file, const char* entry_point_name, void** pp_shader_bytecode, size_t& shader_size);
	PixelShader* CreatePixelShader(const void* p_shader_bytecode, size_t shader_size);

	void ReleaseCompiledShader();


	DeviceContext* GetDeviceContext();

private:
	GraphicsEngine();
protected:
	ID3D11Device* mp_d3d_device;
	D3D_FEATURE_LEVEL m_feature_level;
	IDXGIFactory* mp_dx_factory;
	DeviceContext* mp_ctx;
	ID3D11DeviceContext* mp_imm_ctx;

	ID3DBlob* mp_blob;

	friend class SwapChain;
	friend class VertexBuffer;
	friend class VertexShader;
	friend class PixelShader;
};

