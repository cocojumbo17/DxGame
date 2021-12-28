#include "pch.h"
#include "Matrix4x4.h"
#include <memory>
#include "Vector3d.h"

Matrix4x4::Matrix4x4()
{
	SetIdentity();
}

void Matrix4x4::SetIdentity()
{
	memset(m_matrix, 0, sizeof(float) * 16);
	m_matrix[0][0] = 1;
	m_matrix[1][1] = 1;
	m_matrix[2][2] = 1;
	m_matrix[3][3] = 1;
}

void Matrix4x4::SetTranslation(const Vector3d& translation)
{
	SetIdentity();
	m_matrix[3][0] = translation.m_x;
	m_matrix[3][1] = translation.m_y;
	m_matrix[3][2] = translation.m_z;
}

void Matrix4x4::SetScale(const Vector3d& scale)
{
	SetIdentity();
	m_matrix[0][0] = scale.m_x;
	m_matrix[1][1] = scale.m_y;
	m_matrix[2][2] = scale.m_z;
}

void Matrix4x4::SetOrthoLH(float width, float height, float near_plane, float far_plane)
{
	SetIdentity();
	m_matrix[0][0] = 2.0f / width;
	m_matrix[1][1] = 2.0f / height;
	m_matrix[2][2] = 1.0f / (far_plane - near_plane);
	m_matrix[3][2] = -near_plane / (far_plane - near_plane);
}

void Matrix4x4::operator*=(const Matrix4x4& other)
{
	Matrix4x4 res;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			res.m_matrix[i][j] =	m_matrix[i][0] * other.m_matrix[0][j] +
									m_matrix[i][1] * other.m_matrix[1][j] +
									m_matrix[i][2] * other.m_matrix[2][j] +
									m_matrix[i][3] * other.m_matrix[3][j];
		}
	}
	memcpy(m_matrix, res.m_matrix, sizeof(float) * 16);
}

