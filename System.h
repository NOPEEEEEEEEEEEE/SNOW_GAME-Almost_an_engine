#pragma once
#include <set>

using Entity = std::uint32_t;
//const Entity MAX_ENTITIES = 5000;

class System
{
public:
	std::set<Entity> m_Entities;
};