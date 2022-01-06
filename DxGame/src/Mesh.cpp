#include "pch.h"
#include "MeshManager.h"
#include "Mesh.h"
#include "DirectXTex.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <locale>
#include <codecvt>

#include "VertexMesh.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"


Mesh::Mesh(const wchar_t* full_path)
	: Resource(full_path)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn;
	std::string err;
	std::string filename = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(full_path);
	
	bool res = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str());
	if (!res)
	{
		throw(std::exception("tinyobj::LoadObj returns false"));
	}
	if (!err.empty())
	{
		throw(std::exception("tinyobj::LoadObj returns error"));
	}


	std::vector<VertexMesh> vertices;
	std::vector<unsigned int> indices;
	for (const tinyobj::shape_t& sh : shapes)
	{
		vertices.reserve(sh.mesh.indices.size());
		indices.reserve(sh.mesh.indices.size());
		size_t index_base = 0;
		for (unsigned char num_verts : sh.mesh.num_face_vertices)
		{
			for (unsigned char i = 0; i < num_verts; ++i)
			{
				tinyobj::index_t index = sh.mesh.indices[index_base + i];

				tinyobj::real_t vx = attrib.vertices[index.vertex_index * 3 + 0];
				tinyobj::real_t vy = attrib.vertices[index.vertex_index * 3 + 1];
				tinyobj::real_t vz = attrib.vertices[index.vertex_index * 3 + 2];

				tinyobj::real_t tu = attrib.texcoords[index.texcoord_index * 2 + 0];
				tinyobj::real_t tv = attrib.texcoords[index.texcoord_index * 2 + 1];
				vertices.push_back(VertexMesh(Vector3d(vx,vy,vz), Vector2d(tu,tv)));
				indices.push_back(index_base + i);
			}
			index_base += num_verts;
		}
		void* p_shader_bytecode = nullptr;
		size_t shader_size = 0;
		GraphicsEngine::Instance()->GetVertexMeshLayoutBytecodeAndSize(&p_shader_bytecode, shader_size);
		mp_vb = GraphicsEngine::Instance()->GetRenderSystem()->CreateVertexBuffer(&vertices[0], sizeof(VertexMesh), vertices.size(), p_shader_bytecode, shader_size);
		mp_ib = GraphicsEngine::Instance()->GetRenderSystem()->CreateIndexBuffer(&indices[0], indices.size());
	}
}

Mesh::~Mesh()
{
}

const VertexBufferPtr& Mesh::GetVertexBuffer()
{
	return mp_vb;
}

const IndexBufferPtr& Mesh::GetIndexBuffer()
{
	return mp_ib;
}
