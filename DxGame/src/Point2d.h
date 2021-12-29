#pragma once
class Point2d
{
public:
	Point2d();
	Point2d(int x, int y);
	Point2d(const Point2d& vec);
	~Point2d() {};
	Point2d operator+(const Point2d& other) const;
	Point2d operator-(const Point2d& other) const;
	bool operator ==(const Point2d& other) const;
	bool operator !=(const Point2d& other) const;

public:
	int m_x;
	int m_y;
};


