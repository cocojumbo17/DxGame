#pragma once
#include "Prerequisites.h"

class GraphicsEngine
{
public:
	virtual ~GraphicsEngine();
	bool Init();
	bool Release();
	RenderSystem* GetRenderSystem();
	static GraphicsEngine* Instance();

private:
	GraphicsEngine();
protected:
	RenderSystem* mp_system;
};

