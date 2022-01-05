#include "pch.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "TextureManager.h"

GraphicsEngine* GraphicsEngine::sp_ge = nullptr;

GraphicsEngine::GraphicsEngine()
	:mp_system(nullptr)
{
	try
	{
		mp_system = new RenderSystem();
	}
	catch (const std::exception& e)
	{
		Logger::PrintLog("[ERROR] %s", e.what());
		throw;
	}

	try
	{
		mp_texture_manager = new TextureManager();
	}
	catch (const std::exception& e)
	{
		Logger::PrintLog("[ERROR] %s", e.what());
		throw;
	}
}

GraphicsEngine::~GraphicsEngine()
{
	delete mp_texture_manager;
	delete mp_system;
	GraphicsEngine::sp_ge = nullptr;
}

void GraphicsEngine::Create()
{
	if (GraphicsEngine::sp_ge)
		throw(std::exception("GraphicsEngine is already created."));
	GraphicsEngine::sp_ge = new GraphicsEngine();
}

void GraphicsEngine::Release()
{
	if (!GraphicsEngine::sp_ge)
		return;
	delete GraphicsEngine::sp_ge;
}

RenderSystem* GraphicsEngine::GetRenderSystem()
{
	return mp_system;
}

TextureManager* GraphicsEngine::GetTextureManager()
{
	return mp_texture_manager;
}

GraphicsEngine* GraphicsEngine::Instance()
{
	return GraphicsEngine::sp_ge;
}


