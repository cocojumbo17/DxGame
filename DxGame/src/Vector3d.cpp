#include "pch.h"
#include "Vector3d.h"

Vector3d::Vector3d()
	: m_x(0)
	, m_y(0)
	, m_z(0)
{
}

Vector3d::Vector3d(float x, float y, float z)
	: m_x(x)
	, m_y(y)
	, m_z(z)
{
}

Vector3d::Vector3d(const Vector3d& vec)
	: m_x(vec.m_x)
	, m_y(vec.m_y)
	, m_z(vec.m_z)
{
}

Vector3d Vector3d::lerp(const Vector3d& start, const Vector3d& stop, float part)
{
	Vector3d res;
	res.m_x = start.m_x * (1.0f - part) + stop.m_x * part;
	res.m_y = start.m_y * (1.0f - part) + stop.m_y * part;
	res.m_z = start.m_z * (1.0f - part) + stop.m_z * part;
	return res;
}

Vector3d Vector3d::operator+(const Vector3d other) const
{
	return Vector3d(m_x + other.m_x, m_y + other.m_y, m_z + other.m_z);
}

Vector3d Vector3d::operator*(float s) const
{
	return Vector3d(m_x * s, m_y * s, m_z * s);
}
