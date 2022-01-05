#pragma once
#include "Prerequisites.h"

class GraphicsEngine
{
public:
	static void Create();
	static void Release();
	static GraphicsEngine* Instance();

	RenderSystem* GetRenderSystem();
	TextureManager* GetTextureManager();

private:
	GraphicsEngine();
	virtual ~GraphicsEngine();
protected:
	RenderSystem* mp_system;
	TextureManager* mp_texture_manager;
	static GraphicsEngine* sp_ge;
};

