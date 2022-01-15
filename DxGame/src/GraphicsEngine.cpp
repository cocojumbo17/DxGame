#include "pch.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "TextureManager.h"
#include "MeshManager.h"
#include "DeviceContext.h"
#include "Material.h"

GraphicsEngine* GraphicsEngine::sp_ge = nullptr;

GraphicsEngine::GraphicsEngine()
	: mp_system(nullptr)
	, m_size_of_shader_bytecode(0)

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

	try
	{
		mp_mesh_manager = new MeshManager();
	}
	catch (const std::exception& e)
	{
		Logger::PrintLog("[ERROR] %s", e.what());
		throw;
	}

	void* p_shader_bytecode = nullptr;
	size_t shader_size = 0;
	if (mp_system->CompileVertexShader(L"VertexMeshLayoutShader.hlsl", "vsmain", &p_shader_bytecode, shader_size))
	{
		memcpy(mp_shader_bytecode, p_shader_bytecode, shader_size);
		m_size_of_shader_bytecode = shader_size;
	}
	mp_system->ReleaseCompiledShader();

}

GraphicsEngine::~GraphicsEngine()
{
	delete mp_mesh_manager;
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

MeshManager* GraphicsEngine::GetMeshManager()
{
	return mp_mesh_manager;
}

void GraphicsEngine::GetVertexMeshLayoutBytecodeAndSize(void** pp_bypecode, size_t& size)
{
	*pp_bypecode = mp_shader_bytecode;
	size = m_size_of_shader_bytecode;
}

MaterialPtr GraphicsEngine::CreateMaterial(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path)
{
	MaterialPtr p_mat = nullptr;
	try
	{
		p_mat = std::make_shared<Material>(vertex_shader_path, pixel_shader_path);
	}
	catch (const std::exception& e)
	{
		Logger::PrintLog("[ERROR] %s", e.what());
	}
	return p_mat;	
}

MaterialPtr GraphicsEngine::CreateMaterial(const MaterialPtr& material)
{
	MaterialPtr p_mat = nullptr;
	try
	{
		p_mat = std::make_shared<Material>(material.get());
	}
	catch (const std::exception& e)
	{
		Logger::PrintLog("[ERROR] %s", e.what());
	}
	return p_mat;
}

void GraphicsEngine::SetMaterial(const MaterialPtr& material)
{
	mp_system->SetRasterizerState(material->GetCullMode() == CULL_MODE_FRONT);
	DeviceContextPtr p_ctx = mp_system->GetDeviceContext();
	p_ctx->SetConstantBufferVS(material->mp_cb);
	p_ctx->SetConstantBufferPS(material->mp_cb);
	p_ctx->SetVertexShader(material->mp_vs);
	p_ctx->SetPixelShader(material->mp_ps);
	p_ctx->SetTexturePS(&material->m_textures[0], material->m_textures.size());

}

GraphicsEngine* GraphicsEngine::Instance()
{
	return GraphicsEngine::sp_ge;
}


