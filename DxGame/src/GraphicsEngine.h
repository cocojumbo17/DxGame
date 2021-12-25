#pragma once

class SwapChain;

class GraphicsEngine
{
public:
	virtual ~GraphicsEngine();
	bool Init();
	bool Release();
	static GraphicsEngine* Instance();
	SwapChain* CreateSwapChain();
private:
	GraphicsEngine();
protected:
	ID3D11Device* mp_device;
	D3D_FEATURE_LEVEL m_feature_level;
	ID3D11DeviceContext* mp_imm_ctx;
	IDXGIFactory* mp_dx_factory;
	friend class SwapChain;
};

