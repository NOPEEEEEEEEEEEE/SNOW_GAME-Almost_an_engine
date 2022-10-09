#pragma once
#include "IComponentArray.h"
#include <unordered_map>
template<typename T>
class ComponentArray : public IComponentArray
{
public:
	void InsertData(Entity entity, T component)
	{
		if (m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end())
			throw std::exception("TComponent added to same entity more than once.");

		// Put new entry at end and update the maps
		size_t newIndex = m_Size;
		m_EntityToIndexMap[entity] = newIndex;
		m_IndexToEntityMap[newIndex] = entity;
		m_ComponentArray[newIndex] = component;
		++m_Size;
	}

	void RemoveData(Entity entity)
	{

		if (m_EntityToIndexMap.find(entity) == m_EntityToIndexMap.end())
			throw std::exception("Removing non-existent component.");

		// Copy element at end into deleted element's place to maintain density
		size_t indexOfRemovedEntity = m_EntityToIndexMap[entity];
		size_t indexOfLastElement = m_Size - 1;
		m_ComponentArray[indexOfRemovedEntity] = m_ComponentArray[indexOfLastElement];

		// Update map to point to moved spot
		Entity entityOfLastElement = m_IndexToEntityMap[indexOfLastElement];
		m_EntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		m_IndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

		m_EntityToIndexMap.erase(entity);
		m_IndexToEntityMap.erase(indexOfLastElement);

		--m_Size;
	}

	T& GetData(Entity entity)
	{
		if (m_EntityToIndexMap.find(entity) == m_EntityToIndexMap.end())
			throw std::exception("Retrieving non-existent component.");

		// Return a reference to the entity's component
		return m_ComponentArray[m_EntityToIndexMap[entity]];
	}

	void EntityDestroyed(Entity entity) override
	{
		if (m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end())
		{
			// Remove the entity's component if it existed
			RemoveData(entity);
		}
	}

private:
	// The packed array of components (of generic type T),
	// set to a specified maximum amount, matching the maximum number
	// of entities allowed to exist simultaneously, so that each entity
	// has a unique spot.
	std::array<T, MAX_ENTITIES> m_ComponentArray;

	// Map from an entity ID to an array index.
	std::unordered_map<Entity, size_t> m_EntityToIndexMap;
	
	// Map from an array index to an entity ID.
	std::unordered_map<size_t, Entity> m_IndexToEntityMap;
	
	// Total size of valid entries in the array.
	size_t m_Size;
};