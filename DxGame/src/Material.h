#pragma once
#include "Prerequisites.h"
#include <vector>

enum CULL_MODE
{
	CULL_MODE_FRONT = 0,
	CULL_MODE_BACK
};

class Material
{
public:
	Material(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path);
	Material(const Material* p_other);
	~Material();
	void AddTexture(const TexturePtr& tex);
	void RemoveTexture(size_t index);
	void SetData(void* data, size_t size);
	void SetCullMode(CULL_MODE mode);
	CULL_MODE GetCullMode();

protected:
	VertexShaderPtr mp_vs;
	PixelShaderPtr mp_ps;
	ConstantBufferPtr mp_cb;
	std::vector<TexturePtr> m_textures;
	CULL_MODE m_cull_mode;
	friend class GraphicsEngine;
};

