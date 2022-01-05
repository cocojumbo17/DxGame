#include "pch.h"
#include "Vector2d.h"

Vector2d::Vector2d()
	: m_x(0)
	, m_y(0)
{
}

Vector2d::Vector2d(float x, float y)
	: m_x(x)
	, m_y(y)
{
}

Vector2d::Vector2d(const Vector2d& vec)
	: m_x(vec.m_x)
	, m_y(vec.m_y)
{
}

Vector2d Vector2d::lerp(const Vector2d& start, const Vector2d& stop, float part)
{
	Vector2d res;
	res.m_x = start.m_x * (1.0f - part) + stop.m_x * part;
	res.m_y = start.m_y * (1.0f - part) + stop.m_y * part;
	return res;
}

Vector2d Vector2d::operator+(const Vector2d other) const
{
	return Vector2d(m_x + other.m_x, m_y + other.m_y);
}

Vector2d Vector2d::operator*(float s) const
{
	return Vector2d(m_x * s, m_y * s);
}
