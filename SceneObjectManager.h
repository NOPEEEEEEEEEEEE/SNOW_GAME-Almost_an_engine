#pragma once
#include "EntityComponentSystemManager.h"
#include "System.h"
#include "Vector3D.h"
#include "Vector2D.h"
#include "Rotator3D.h"
#include "Object.h"
#include <iostream>

//extern EntityComponentSystemManager m_EntityComponentSystemManager;
//std::vector<Entity> m_Entities(MAX_ENTITIES);

class SceneObjectManager: public System
{
public:
	SceneObjectManager()
	{
      m_Entity_Index.resize(MAX_ENTITIES);
	}
	~SceneObjectManager() 
	{

	}
	 void Tick(float DeltaTime)
	{
		
		 for (auto objects : m_Objects)
		 { 
			 objects->Tick( DeltaTime);
		 }
	
	}
	 void BeginPlay()
	{
		
		for (auto objects : m_Objects)
		{
			objects->BeginPlay();
		
		}
	
	}

	 void registerObject(Object* obj, const char* Name)
	 {
		
		 obj->SetEntityID(m_EntityComponentSystemManager->CreateEntity());
		 obj->SetName(Name);
		 obj->SetECS(m_EntityComponentSystemManager);
		
		 m_Max_Objects++;
		 int index = m_Max_Objects - 1;
		 m_Objects.resize(m_Max_Objects);
		 if (m_Max_Objects != 0)
		 {
			 m_Objects[index] = obj;
			 m_Entity_Index[obj->GetEntityID()] = index;
		 }
		 
	 }
	 void deleteObjectFromManager(Object* obj)
	 {
		
      //Replace the deleted object with the last element of the vector
		 Entity m_last_obj_added;
		 m_last_obj_added = m_Objects[int(double(m_Max_Objects) - 1)]->GetEntityID();
		 
		 m_Objects[m_Entity_Index[obj->GetEntityID()]] = m_Objects[int(double(m_Max_Objects) - 1)];
		 m_Entity_Index[m_last_obj_added] = m_Entity_Index[obj->GetEntityID()];

		 m_Objects[int(double(m_Max_Objects) - 1)] = nullptr;
		 m_Entity_Index[obj->GetEntityID()] = 0;
		 ////Delete from the ECS
		 m_EntityComponentSystemManager->DestroyEntity(obj->GetEntityID());


		 m_Max_Objects--;
		 m_Objects.resize(m_Max_Objects);

	 }
	 void deleteObject(Object* obj)
	{
		delete obj;
		obj = nullptr;

	}

	 void setUp(EntityComponentSystemManager* ECS)
	 {
		 m_EntityComponentSystemManager = ECS;
	 }

private:
	
	int m_Max_Objects = 0;
	std::vector<Object*> m_Objects{};
	std::vector<int> m_Entity_Index{};
	EntityComponentSystemManager* m_EntityComponentSystemManager = nullptr;


    void AddObject(Object* obj) 
	{
		m_Max_Objects++;
		int index = m_Max_Objects - 1;
		m_Objects.resize(m_Max_Objects);
		if (m_Max_Objects != 0)
		{
			m_Objects[index] = obj;
			m_Entity_Index[obj->GetEntityID()] = index;
		}
	}

friend class SceneActorManager;
friend class ScenePawnManager;
friend class SceneCharacterManager;

};