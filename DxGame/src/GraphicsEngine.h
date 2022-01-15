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
	MeshManager* GetMeshManager();
	void GetVertexMeshLayoutBytecodeAndSize(void** pp_bypecode, size_t& size);

	MaterialPtr CreateMaterial(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path);
	MaterialPtr CreateMaterial(const MaterialPtr& material);
	void SetMaterial(const MaterialPtr& material);
private:
	GraphicsEngine();
	virtual ~GraphicsEngine();
protected:
	RenderSystem* mp_system;
	TextureManager* mp_texture_manager;
	MeshManager* mp_mesh_manager;
	byte mp_shader_bytecode[1024];
	size_t m_size_of_shader_bytecode;
	static GraphicsEngine* sp_ge;
};

