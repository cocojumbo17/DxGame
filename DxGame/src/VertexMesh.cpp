#include "pch.h"
#include "VertexMesh.h"

VertexMesh::VertexMesh()
	: m_position()
	, m_texcoord()
{
}

VertexMesh::VertexMesh(const Vector3d& pos, const Vector2d& tex)
	: m_position(pos)
	, m_texcoord(tex)
{
}

VertexMesh::VertexMesh(const VertexMesh& v)
	: m_position(v.m_position)
	, m_texcoord(v.m_texcoord)
{
}
