#pragma once
#include<memory>
#include"Vector3D.h"
#include"Vector4D.h"

//#include <iostream>

class Matrix4x4
{
public:
	Matrix4x4()
	{


	}

	void setIdentity()
	{
		memset(m_matrix, 0, sizeof(float) * 16);
		m_matrix[0][0] = 1;
		m_matrix[1][1] = 1;
		m_matrix[2][2] = 1;
		m_matrix[3][3] = 1;
	}

	void setTranslation(const Vector3D& translation)
	{
		m_matrix[3][0] = translation.m_x;
		m_matrix[3][1] = translation.m_y;
		m_matrix[3][2] = translation.m_z;

	}
	void setScale(const Vector3D& scale)
	{
		m_matrix[0][0] = scale.m_x;
		m_matrix[1][1] = scale.m_y;
		m_matrix[2][2] = scale.m_z;

	}

	void setRotationDegreesX(float x)
	{
		    x = x * float(3.14 / 180); //convert to radians
			m_matrix[1][1] = float(cos(x));
			m_matrix[1][2] = float(sin(x));
			m_matrix[2][1] = float(-sin(x));
			m_matrix[2][2]= float(cos(x));

	}
	void setRotationDegreesY(float y)
	{
			y = y * float(3.14 / 180); //convert to radians
			m_matrix[0][0] = float(cos(y));
			m_matrix[2][0] = float(sin(y));
			m_matrix[0][2] = float(-sin(y));
			m_matrix[2][2] = float(cos(y));

	}
	void setRotationDegreesZ(float z)
	{
			z = z * float(3.14 / 180); //convert to radians
			m_matrix[0][0] = float(cos(z));
			m_matrix[0][1] = float(sin(z));
			m_matrix[1][0] = float(-sin(z));
			m_matrix[1][1] = float(cos(z));

	}
	void setRotationRadiansX(float x)
	{
			m_matrix[1][1] = float(cos(x));
			m_matrix[1][2] = float(sin(x));
			m_matrix[2][1] = float(-sin(x));
			m_matrix[2][2] = float(cos(x));

	}
	void setRotationRadiansY(float y)
	{
			m_matrix[0][0] = float(cos(y));
			m_matrix[2][0] = float(sin(y));
			m_matrix[0][2] = float(-sin(y));
			m_matrix[2][2] = float(cos(y));

	}
	void setRotationRadiansZ(float z)
	{
			m_matrix[0][0] = float(cos(z));
			m_matrix[0][1] = float(sin(z));
			m_matrix[1][0] = float(-sin(z));
			m_matrix[1][1] = float(cos(z));

	}

	float getDeterminant()
	{
		Vector4D minor, v1, v2, v3;
		float det;

		v1 = Vector4D(this->m_matrix[0][0], this->m_matrix[1][0], this->m_matrix[2][0], this->m_matrix[3][0]);
		v2 = Vector4D(this->m_matrix[0][1], this->m_matrix[1][1], this->m_matrix[2][1], this->m_matrix[3][1]);
		v3 = Vector4D(this->m_matrix[0][2], this->m_matrix[1][2], this->m_matrix[2][2], this->m_matrix[3][2]);


		minor.cross(v1, v2, v3);
		det = -(this->m_matrix[0][3] * minor.m_x + this->m_matrix[1][3] * minor.m_y + this->m_matrix[2][3] * minor.m_z +
			this->m_matrix[3][3] * minor.m_w);
		return det;
	}

	void inverse()
	{
		int a, i, j;
		Matrix4x4 out;
		Vector4D v, vec[3];
		float det = 0.0f;

		det = this->getDeterminant();
		if (!det) return;
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (j != i)
				{
					a = j;
					if (j > i) a = a - 1;
					vec[a].m_x = (this->m_matrix[j][0]);
					vec[a].m_y = (this->m_matrix[j][1]);
					vec[a].m_z = (this->m_matrix[j][2]);
					vec[a].m_w = (this->m_matrix[j][3]);
				}
			}
			v.cross(vec[0], vec[1], vec[2]);

			out.m_matrix[0][i] = float(pow(-1.0f, i) * v.m_x / det);
			out.m_matrix[1][i] = float(pow(-1.0f, i) * v.m_y / det);
			out.m_matrix[2][i] = float(pow(-1.0f, i) * v.m_z / det);
			out.m_matrix[3][i] = float(pow(-1.0f, i) * v.m_w / det);
		}

		this->setMatrix(out);
	}

	void operator *= (const Matrix4x4& matrix)
	{
		Matrix4x4 out;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				out.m_matrix[i][j] = m_matrix[i][0] * matrix.m_matrix[0][j] + m_matrix[i][1] * matrix.m_matrix[1][j]
					+ m_matrix[i][2] * matrix.m_matrix[2][j] + m_matrix[i][3] * matrix.m_matrix[3][j];
			}

		}
		memcpy(m_matrix, out.m_matrix, sizeof(float) * 16);

	}
	Vector3D getXDirection()
	{
		return Vector3D(m_matrix[0][0], m_matrix[0][1], m_matrix[0][2]);
	}
	Vector3D getYDirection()
	{
		return Vector3D(m_matrix[1][0], m_matrix[1][1], m_matrix[1][2]);
	}
	Vector3D getZDirection()
	{
		return Vector3D(m_matrix[2][0], m_matrix[2][1], m_matrix[2][2]);
	}

	Vector3D getTranslation()
	{
		return Vector3D(m_matrix[3][0], m_matrix[3][1], m_matrix[3][2]);
	}


	void setMatrix(const Matrix4x4& matrix)
	{
		::memcpy(m_matrix, matrix.m_matrix, sizeof(float) * 16);
	}

	void setPerspectiveFovLH(float fov, float aspect, float znear, float zfar)
	{
		float yscale = float(1.0f / tan(fov / 2.0f));
		float xscale = yscale / aspect;

		m_matrix[0][0] = xscale;
		m_matrix[1][1] = yscale;
		m_matrix[2][2] = zfar / (zfar - znear);
		m_matrix[2][3] = 1.0f;
		m_matrix[3][2] = (-znear * zfar) / (zfar - znear);


	}

	void setOrthoLH(float width, float height, float near_plane, float far_plane)
	{
		setIdentity();
		m_matrix[0][0] = 2.0f / width;
		m_matrix[1][1] = 2.0f / height;
		m_matrix[2][2] = 1.0f / (far_plane - near_plane);
		m_matrix[3][2] = -(near_plane / (far_plane - near_plane));
	}


	~Matrix4x4()
	{


	}


	float m_matrix[4][4] = {};

};
