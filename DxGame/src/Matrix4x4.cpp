#include "pch.h"
#include "Matrix4x4.h"
#include <memory>
#include "Vector3d.h"
#include "Vector4d.h"

Matrix4x4::Matrix4x4()
{
	SetIdentity();
}

void Matrix4x4::SetMatrix(const Matrix4x4& other)
{
	memcpy(m_matrix, other.m_matrix, sizeof(float) * 16);
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
	m_matrix[3][0] = translation.m_x;
	m_matrix[3][1] = translation.m_y;
	m_matrix[3][2] = translation.m_z;
}

void Matrix4x4::SetScale(const Vector3d& scale)
{
	m_matrix[0][0] = scale.m_x;
	m_matrix[1][1] = scale.m_y;
	m_matrix[2][2] = scale.m_z;
}

void Matrix4x4::SetRotationX(float angle)
{
	float c = cos(angle);
	float s = sin(angle);
	m_matrix[1][1] = c;
	m_matrix[2][2] = c;
	m_matrix[1][2] = s;
	m_matrix[2][1] = -s;
}

void Matrix4x4::SetRotationY(float angle)
{
	float c = cos(angle);
	float s = sin(angle);
	m_matrix[0][0] = c;
	m_matrix[2][2] = c;
	m_matrix[0][2] = -s;
	m_matrix[2][0] = s;
}

void Matrix4x4::SetRotationZ(float angle)
{
	float c = cos(angle);
	float s = sin(angle);
	m_matrix[0][0] = c;
	m_matrix[1][1] = c;
	m_matrix[0][1] = s;
	m_matrix[1][0] = -s;
}

void Matrix4x4::SetOrthoLH(float width, float height, float near_plane, float far_plane)
{
	m_matrix[0][0] = 2.0f / width;
	m_matrix[1][1] = 2.0f / height;
	m_matrix[2][2] = 1.0f / (far_plane - near_plane);
	m_matrix[3][2] = -near_plane / (far_plane - near_plane);
}

void Matrix4x4::SetPerspectiveFovLH(float fov, float aspect, float near_plane, float far_plane)
{
	float yscale = 1.0f / tan(fov / 2.0f);
	float xscale = yscale / aspect;
	m_matrix[0][0] = xscale;
	m_matrix[1][1] = yscale;
	m_matrix[2][2] = far_plane / (far_plane - near_plane);
	m_matrix[2][3] = 1.0f;
	m_matrix[3][2] = (-near_plane*far_plane) / (far_plane - near_plane);
	m_matrix[3][3] = 0.0f;
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
	SetMatrix(res);
}

void Matrix4x4::Inverse()
{
	int a, i, j;
	Matrix4x4 out;
	Vector4d v, vec[3];
	float det = 0.0f;

	det = CalcDeterminant();
	if (!det) 
		return;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (j != i)
			{
				a = j;
				if (j > i)
					a = a - 1;
				vec[a].m_x = (m_matrix[j][0]);
				vec[a].m_y = (m_matrix[j][1]);
				vec[a].m_z = (m_matrix[j][2]);
				vec[a].m_w = (m_matrix[j][3]);
			}
		}
		v.Cross(vec[0], vec[1], vec[2]);

		float sign = ((i % 2) == 1) ? -1.0f : 1.0f;
		out.m_matrix[0][i] = sign * v.m_x / det;
		out.m_matrix[1][i] = sign * v.m_y / det;
		out.m_matrix[2][i] = sign * v.m_z / det;
		out.m_matrix[3][i] = sign * v.m_w / det;
	}

	SetMatrix(out);
}

float Matrix4x4::CalcDeterminant()
{
	Vector4d minor, v1, v2, v3;
	float det;

	v1 = Vector4d(m_matrix[0][0], m_matrix[1][0], m_matrix[2][0], m_matrix[3][0]);
	v2 = Vector4d(m_matrix[0][1], m_matrix[1][1], m_matrix[2][1], m_matrix[3][1]);
	v3 = Vector4d(m_matrix[0][2], m_matrix[1][2], m_matrix[2][2], m_matrix[3][2]);


	minor.Cross(v1, v2, v3);
	det = -(m_matrix[0][3] * minor.m_x + m_matrix[1][3] * minor.m_y + m_matrix[2][3] * minor.m_z + m_matrix[3][3] * minor.m_w);
	return det;

}

Vector3d Matrix4x4::GetXDirection()
{
	return Vector3d(m_matrix[0][0], m_matrix[0][1], m_matrix[0][2]);
}

Vector3d Matrix4x4::GetYDirection()
{
	return Vector3d(m_matrix[1][0], m_matrix[1][1], m_matrix[1][2]);
}

Vector3d Matrix4x4::GetZDirection()
{
	return Vector3d(m_matrix[2][0], m_matrix[2][1], m_matrix[2][2]);
}

Vector3d Matrix4x4::GetTranslation()
{
	return Vector3d(m_matrix[3][0], m_matrix[3][1], m_matrix[3][2]);
}
