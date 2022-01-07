#include "pch.h"
#include "VertexMesh.h"

VertexMesh::VertexMesh()
	: m_position()
	, m_texcoord()
	, m_normal()
{
}

VertexMesh::VertexMesh(const Vector3d& pos, const Vector2d& tex, const Vector3d& normal)
	: m_position(pos)
	, m_texcoord(tex)
	, m_normal(normal)
{
}

VertexMesh::VertexMesh(const VertexMesh& v)
	: m_position(v.m_position)
	, m_texcoord(v.m_texcoord)
	, m_normal(v.m_normal)
{
}
