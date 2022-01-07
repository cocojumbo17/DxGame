#pragma once
class Vector3d;

class Vector4d
{
public:
	Vector4d();
	Vector4d(float x, float y, float z, float w);
	Vector4d(const Vector4d& vec);
	Vector4d(const Vector3d& vec);
	~Vector4d() {};
	void Cross(Vector4d& v1, Vector4d& v2, Vector4d& v3);
public:
	float m_x;
	float m_y;
	float m_z;
	float m_w;
};

