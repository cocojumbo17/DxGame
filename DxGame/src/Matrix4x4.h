#pragma once

class Vector3d;

class Matrix4x4
{
public:
	Matrix4x4();
	~Matrix4x4() {};
	void SetMatrix(const Matrix4x4& other);
	void SetIdentity();
	void SetTranslation(const Vector3d& translation);
	void SetScale(const Vector3d& scale);
	void SetRotationX(float angle);
	void SetRotationY(float angle);
	void SetRotationZ(float angle);
	void SetOrthoLH(float width, float height, float near_plane, float far_plane);
	void SetPerspectiveFovLH(float fov, float aspect, float near_plane, float far_plane);
	void operator *=(const Matrix4x4& other);
	void Inverse();
	float CalcDeterminant();
	Vector3d GetXDirection();
	Vector3d GetYDirection();
	Vector3d GetZDirection();
	Vector3d GetTranslation();
protected:
	float m_matrix[4][4];
};

