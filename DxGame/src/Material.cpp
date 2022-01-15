#include "pch.h"
#include "Material.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"

Material::Material(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path)
	: m_cull_mode(CULL_MODE_FRONT)
{
	RenderSystem* p_rs = GraphicsEngine::Instance()->GetRenderSystem();
	void* p_shader_bytecode = nullptr;
	size_t shader_size = 0;
	if (p_rs->CompileVertexShader(vertex_shader_path, "vsmain", &p_shader_bytecode, shader_size))
	{
		mp_vs = p_rs->CreateVertexShader(p_shader_bytecode, shader_size);
	}
	p_rs->ReleaseCompiledShader();

	if (p_rs->CompilePixelShader(pixel_shader_path, "psmain", &p_shader_bytecode, shader_size))
	{
		mp_ps = p_rs->CreatePixelShader(p_shader_bytecode, shader_size);
	}
	p_rs->ReleaseCompiledShader();
}

Material::Material(const Material* p_other)
{
	mp_ps = p_other->mp_ps;
	mp_vs = p_other->mp_vs;
	m_cull_mode = p_other->m_cull_mode;
}

Material::~Material()
{
}

void Material::AddTexture(const TexturePtr& tex)
{
	m_textures.push_back(tex);
}

void Material::RemoveTexture(size_t index)
{
	if (index < m_textures.size())
		m_textures.erase(m_textures.begin() + index);
}

void Material::SetData(void* data, size_t size)
{
	RenderSystem* p_rs = GraphicsEngine::Instance()->GetRenderSystem();
	if (!mp_cb)
		mp_cb = p_rs->CreateConstantBuffer(data, size);
	else
		mp_cb->Update(p_rs->GetDeviceContext(), data);
}

void Material::SetCullMode(CULL_MODE mode)
{
	m_cull_mode = mode;
}

CULL_MODE Material::GetCullMode()
{
	return m_cull_mode;
}
