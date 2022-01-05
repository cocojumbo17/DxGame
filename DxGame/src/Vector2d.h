#pragma once
class Vector2d
{
public:
	Vector2d();
	Vector2d(float x, float y);
	Vector2d(const Vector2d& vec);
	~Vector2d() {};
	static Vector2d lerp(const Vector2d& start, const Vector2d& stop, float part);
	Vector2d operator + (const Vector2d other) const;
	Vector2d operator * (float s) const;
public:
	float m_x;
	float m_y;
};

