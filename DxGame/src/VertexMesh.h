#pragma once
#include "Vector3d.h"
#include "Vector2d.h"

class VertexMesh
{
public:
	VertexMesh();
	VertexMesh(const Vector3d& pos, const Vector2d& tex);
	VertexMesh(const VertexMesh& vec);
	~VertexMesh() {};
public:
	Vector3d m_position;
	Vector2d m_texcoord;
};

