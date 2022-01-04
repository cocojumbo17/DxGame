#pragma once
#include "Prerequisites.h"

class GraphicsEngine
{
public:
	static void Create();
	static void Release();
	static GraphicsEngine* Instance();

	RenderSystem* GetRenderSystem();

private:
	GraphicsEngine();
	virtual ~GraphicsEngine();
protected:
	RenderSystem* mp_system;
	static GraphicsEngine* sp_ge;
};

