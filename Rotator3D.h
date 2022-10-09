#pragma once
#include "Vector3D.h"

class Rotator3D
{
public:

	Rotator3D() : m_x(0),m_y(0),m_z(0)
	{
	}

	Rotator3D(float x, float y, float z) : m_x(x), m_y(y), m_z(z)
	{
	}
	Rotator3D(const Rotator3D& rotator) : m_x(rotator.m_x), m_y(rotator.m_y), m_z(rotator.m_z)
	{
	}

	
	static Rotator3D lerpRotator(const Rotator3D& start, const Rotator3D& end, float delta)
	{
		Vector3D r;
		r = Vector3D::lerp(Vector3D(start.m_x, start.m_y, start.m_z), Vector3D(end.m_x, end.m_y, end.m_z),delta);
		return Rotator3D(r.m_x, r.m_y, r.m_z);
	}

	Rotator3D operator *(float num) const
	{
		return Rotator3D(m_x * num, m_y * num,m_z * num);
	}
	Rotator3D operator +(const Rotator3D& rot) const
	{
		return Rotator3D(m_x + rot.m_x, 
		                 m_y + rot.m_y, 
			             m_z + rot.m_z);
	}

	Rotator3D operator -(const Rotator3D& rot) const
	{
		return Rotator3D(m_x - rot.m_x,
			             m_y - rot.m_y,
			             m_z - rot.m_z);
	}
	Vector3D GetDegreeValue()
	{

		return Vector3D(m_x, m_y, m_z);
	}

	Vector3D GetRadianValue()
	{

		return ConvertDegreesToRadians();
	}
	
	~Rotator3D()
	{



	}

	
	 float m_x, m_y, m_z;
	

private:

     Vector3D ConvertDegreesToRadians()
	{
		 float pi = 3.14f;
		 Vector3D RadianValue;
		 RadianValue.m_x = m_x * (pi / 180);
		 RadianValue.m_y = m_y * (pi / 180);
		 RadianValue.m_z = m_z * (pi / 180);
		 return RadianValue;

	}


	 
};