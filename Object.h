#pragma once
#include "EntityComponentSystemManager.h"
#include <iostream>

class SceneObjectManager;

class Object
{

public:
	Object()
	{

	}
	
	
	~Object()
	{
		//delete this;
	}

	virtual void Tick(float DeltaTime) 
	{
		

	}
	virtual void BeginPlay() 
	{
		

	}
	
	void SetEntityID(Entity IDToSet)
	{
		//std::cout << "ID ok";
		m_ID = IDToSet;
	}

	Entity GetEntityID()
	{
		return m_ID;
	}
     void SetName(const char* Name)
	{
		m_name = std::string(Name);
	}
	 void SetECS(EntityComponentSystemManager* ECS)
	 {
		 m_EntityComponentSystemManager = ECS;
	 }
	
	const char* GetName()
	{
		return m_name.c_str();
	}




protected:
	Entity m_ID{};
	std::string m_name{};
	EntityComponentSystemManager* m_EntityComponentSystemManager = nullptr;

};