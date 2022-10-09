#pragma once
#include "vector3D.h"
#include <random>
#include <time.h> 

static float lerp(float start, float end, float delta)
{
	return start * (1.0f - delta) + end * delta;
}

static float distance(const Vector3D& vec1, const Vector3D& vec2)
{
	float distance;

	distance = sqrt(pow(vec2.m_x - vec1.m_x, 2) + pow(vec2.m_y - vec1.m_y, 2) + pow(vec2.m_z - vec1.m_z, 2));

	return distance;
}

//if there is no need for the precise distance, we can use the sqare to avoid using sqrt
static float sqaredDistance(const Vector3D& vec1, const Vector3D& vec2)
{
	float distanceS;

	distanceS = pow(vec2.m_x - vec1.m_x, 2) + pow(vec2.m_y - vec1.m_y, 2) + pow(vec2.m_z - vec1.m_z, 2);

	return distanceS;
}
