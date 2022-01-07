#include "pch.h"
#include "Vector4d.h"
#include "Vector3d.h"

Vector4d::Vector4d()
	: m_x(0)
	, m_y(0)
	, m_z(0)
	, m_w(0)
{
}

Vector4d::Vector4d(float x, float y, float z, float w)
	: m_x(x)
	, m_y(y)
	, m_z(z)
	, m_w(w)
{
}

Vector4d::Vector4d(const Vector4d& vec)
	: m_x(vec.m_x)
	, m_y(vec.m_y)
	, m_z(vec.m_z)
	, m_w(vec.m_w)
{
}

Vector4d::Vector4d(const Vector3d& vec)
	: m_x(vec.m_x)
	, m_y(vec.m_y)
	, m_z(vec.m_z)
	, m_w(1.0f)
{
}

void Vector4d::Cross(Vector4d& v1, Vector4d& v2, Vector4d& v3)
{
	m_x = v1.m_y * (v2.m_z * v3.m_w - v3.m_z * v2.m_w) - v1.m_z * (v2.m_y * v3.m_w - v3.m_y * v2.m_w) + v1.m_w * (v2.m_y * v3.m_z - v2.m_z * v3.m_y);
	m_y = -(v1.m_x * (v2.m_z * v3.m_w - v3.m_z * v2.m_w) - v1.m_z * (v2.m_x * v3.m_w - v3.m_x * v2.m_w) + v1.m_w * (v2.m_x * v3.m_z - v3.m_x * v2.m_z));
	m_z = v1.m_x * (v2.m_y * v3.m_w - v3.m_y * v2.m_w) - v1.m_y * (v2.m_x * v3.m_w - v3.m_x * v2.m_w) + v1.m_w * (v2.m_x * v3.m_y - v3.m_x * v2.m_y);
	m_w = -(v1.m_x * (v2.m_y * v3.m_z - v3.m_y * v2.m_z) - v1.m_y * (v2.m_x * v3.m_z - v3.m_x * v2.m_z) + v1.m_z * (v2.m_x * v3.m_y - v3.m_x * v2.m_y));
}
