#pragma once
class Vector3d
{
public:
	Vector3d();
	Vector3d(float x, float y, float z);
	Vector3d(const Vector3d& vec);
	~Vector3d() {};
	static Vector3d lerp(const Vector3d& start, const Vector3d& stop, float part);
public:
	float m_x;
	float m_y;
	float m_z;
};

