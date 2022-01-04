#include "pch.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"


GraphicsEngine::GraphicsEngine()
	:mp_system(nullptr)
{
}

GraphicsEngine::~GraphicsEngine()
{
}

bool GraphicsEngine::Init()
{
	mp_system = new RenderSystem();
	mp_system->Init();
	return true;
}

bool GraphicsEngine::Release()
{
	SAFE_RELEASE(mp_system);
	return true;
}

RenderSystem* GraphicsEngine::GetRenderSystem()
{
	return mp_system;
}

GraphicsEngine* GraphicsEngine::Instance()
{
	static GraphicsEngine inst;
	return &inst;
}


