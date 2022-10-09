#pragma once
#include "EntityManager.h"

using Entity = std::uint32_t;
//const Entity MAX_ENTITIES = 5000;

class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;
};