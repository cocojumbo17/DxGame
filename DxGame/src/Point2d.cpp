#include "pch.h"
#include "Point2d.h"

Point2d::Point2d()
	: m_x(0)
	, m_y(0)
{
}

Point2d::Point2d(int x, int y)
	: m_x(x)
	, m_y(y)
{
}

Point2d::Point2d(const Point2d& vec)
	: m_x(vec.m_x)
	, m_y(vec.m_y)
{
}

Point2d Point2d::operator-(const Point2d& other) const
{
	Point2d res(*this);
	res.m_x -= other.m_x;
	res.m_y -= other.m_y;
	return res;
}

Point2d Point2d::operator+(const Point2d& other) const
{
	Point2d res(*this);
	res.m_x += other.m_x;
	res.m_y += other.m_y;
	return res;
}

bool Point2d::operator!=(const Point2d& other) const
{
	if (m_x != other.m_x)
		return true;
	if (m_y != other.m_y)
		return true;
	return false;
}

bool Point2d::operator==(const Point2d& other) const
{
	return !(*this != other);
}
