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
	std::string mat_folder_name = filename.substr(0, filename.find_last_of("\\/"));

	bool res = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str(), mat_folder_name.c_str());
	if (!res)
	{
		std::stringstream ss;
		ss << "tinyobj::LoadObj of " << filename << " returns false.";
		throw(std::exception(ss.str().c_str()));
	}
	if (!err.empty())
	{
		std::stringstream ss;
		ss << "tinyobj::LoadObj of " << filename << " returns error: \"" << err << "\".";
		throw(std::exception("tinyobj::LoadObj returns error"));
	}
	if (!warn.empty())
	{
		Logger::PrintLog("[WARNING] During loading mesh of %s there is a warning:\"%s\"", filename.c_str(), warn.c_str());
	}

	std::vector<VertexMesh> vertices;
	std::vector<unsigned int> indices;

	size_t global_num_vertices = 0;
	for (const tinyobj::shape_t& sh : shapes)
	{
		global_num_vertices += sh.mesh.indices.size();
	}

	vertices.reserve(global_num_vertices);
	indices.reserve(global_num_vertices);
	m_mat_slots.resize(materials.size());

	size_t mat_index = 0;
	size_t global_index_base = 0;
	for (const tinyobj::material_t& m : materials)
	{
		m_mat_slots[mat_index].m_material_id = mat_index;
		m_mat_slots[mat_index].m_start_index = global_index_base;

		for (const tinyobj::shape_t& sh : shapes)
		{
			size_t index_base = 0;
			size_t face_index = 0;
			for (unsigned char num_verts : sh.mesh.num_face_vertices)
			{
				if (sh.mesh.material_ids[face_index++] != mat_index)
					continue;
				for (unsigned char i = 0; i < num_verts; ++i)
				{
					tinyobj::index_t index = sh.mesh.indices[index_base + i];

					tinyobj::real_t vx = attrib.vertices[index.vertex_index * 3 + 0];
					tinyobj::real_t vy = attrib.vertices[index.vertex_index * 3 + 1];
					tinyobj::real_t vz = attrib.vertices[index.vertex_index * 3 + 2];

					tinyobj::real_t nx = attrib.normals[index.normal_index * 3 + 0];
					tinyobj::real_t ny = attrib.normals[index.normal_index * 3 + 1];
					tinyobj::real_t nz = attrib.normals[index.normal_index * 3 + 2];

					tinyobj::real_t tu = attrib.texcoords[index.texcoord_index * 2 + 0];
					tinyobj::real_t tv = attrib.texcoords[index.texcoord_index * 2 + 1];
					vertices.push_back(VertexMesh(Vector3d(vx, vy, vz), Vector2d(tu, tv), Vector3d(nx, ny, nz)));
					indices.push_back(global_index_base + index_base + i);
				}
				index_base += num_verts;
			}
			global_index_base += index_base;
		}
		m_mat_slots[mat_index].m_num_indices = global_index_base - m_mat_slots[mat_index].m_start_index;
		++mat_index;
	}

	void* p_shader_bytecode = nullptr;
	size_t shader_size = 0;
	GraphicsEngine::Instance()->GetVertexMeshLayoutBytecodeAndSize(&p_shader_bytecode, shader_size);
	mp_vb = GraphicsEngine::Instance()->GetRenderSystem()->CreateVertexBuffer(&vertices[0], sizeof(VertexMesh), vertices.size(), p_shader_bytecode, shader_size);
	mp_ib = GraphicsEngine::Instance()->GetRenderSystem()->CreateIndexBuffer(&indices[0], indices.size());
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

const MaterialSlot& Mesh::GetMaterialSlot(size_t index)
{
	if (index >= m_mat_slots.size())
		return MaterialSlot();
	return m_mat_slots[index];
}

size_t Mesh::GetNumMaterialSlots()
{
	return m_mat_slots.size();
}
