#pragma once
#include "EntityComponentSystemManager.h"
#include "SceneObjectManager.h"
#include "System.h"
#include "Vector3D.h"
#include "Vector2D.h"
#include "Rotator3D.h"
#include "Mesh.h"
#include "Material.h"
#include "Actor.h"
#include "Socket.h"
#include "CollisionDetectionSystem.h"
#include "GraphicsEngine.h"

#include <iostream>

class SceneActorManager : public SceneObjectManager
{
public:
	SceneActorManager()
	{
		m_Entity_Index.resize(MAX_ENTITIES);
	}
	~SceneActorManager()
	{
		
	}
     void Tick(float DeltaTime) 
	{
		if(m_Max_Actors!=0)
		for (auto actors : m_Actors)
		{
			actors->Tick(DeltaTime);
		}
	
	}
	
	void BeginPlay()
	{
	
		if (m_Max_Actors != 0)
		for (auto actors : m_Actors)
		{
			
			actors->BeginPlay();
			
		}
		// 

	}
	void setCollisionSystem(std::shared_ptr<CollisionDetectionSystem> collision_system)
	{
		m_collision_system = collision_system;


	}
	void setUp(EntityComponentSystemManager* ECS, std::shared_ptr<SceneObjectManager> objectManager)
	{
		m_EntityComponentSystemManager = ECS;
		m_scene_object_manager = objectManager;
	}
	void registerObject(Actor* act, const char* Name, const wchar_t* texture_file, const wchar_t* normal_texture_file, const wchar_t* specular_texture_file,
		const wchar_t* mesh_file, const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path, Vector3D BBsize,CULL_MODE cull_mode)
	{
		
		act->SetEntityID(m_EntityComponentSystemManager->CreateEntity());
		act->SetName(Name);
		act->SetBoundingBoxSize(BBsize);
		act->SetCollisionSystem(m_collision_system);
		act->SetECS(m_EntityComponentSystemManager);


		//ADDING THE TRANSFORM COMPONENT
		TransformComponent tr;
		tr.position = Vector3D(1, 1, 1);
		tr.rotation = Rotator3D(0, 0, 0);
		tr.scale = Vector3D(1, 1, 1);
		m_EntityComponentSystemManager->AddComponent(act->GetEntityID(), tr);

		//ADDING THE MESH COMPONENT

		TexturePtr temp_texture;
		MeshComponent meshComponent;
		meshComponent.mesh = GraphicsEngine::GetGraphicsEngine()->getMeshManager()->createMeshFromFile(mesh_file);
		meshComponent.material = GraphicsEngine::GetGraphicsEngine()->createMaterial(vertex_shader_path, pixel_shader_path);
        temp_texture = GraphicsEngine::GetGraphicsEngine()->getTextureManager()->createTextureFromFile(texture_file);
		meshComponent.material->addTexture(temp_texture);
		if(normal_texture_file)
		{ 
		temp_texture = GraphicsEngine::GetGraphicsEngine()->getTextureManager()->createTextureFromFile(normal_texture_file);
		meshComponent.material->addTexture(temp_texture);
        }

		if (specular_texture_file)
		{
			temp_texture = GraphicsEngine::GetGraphicsEngine()->getTextureManager()->createTextureFromFile(specular_texture_file);
			meshComponent.material->addTexture(temp_texture);
		}

		meshComponent.material->setCullMode(cull_mode);


		m_EntityComponentSystemManager->AddComponent(act->GetEntityID(), meshComponent);

		//ADDING PHYSICS
		PhysicsComponent phys{};
		m_EntityComponentSystemManager->AddComponent(act->GetEntityID(), phys);
		//ADDING BOUNDINGBOX
		BoundingBox BBox{};
		BBox.SetOwner(act);
		m_EntityComponentSystemManager->AddComponent(act->GetEntityID(), BBox);

		m_Max_Actors++;
		int index = m_Max_Actors - 1;
		m_Actors.resize(m_Max_Actors);
		if (m_Max_Actors != 0)
		{
			m_Actors[index] = act;
			m_Entity_Index[act->GetEntityID()] = index;
		}
		m_scene_object_manager->AddObject(act);


	}
	void InstantiateActor(Actor* act_to_create,Actor* act_to_copy)
	{

		act_to_create->SetEntityID(m_EntityComponentSystemManager->CreateEntity());
	    act_to_create->SetName(act_to_copy->GetName());
		act_to_create->SetBoundingBoxSize(act_to_copy->GetBoundingBoxSize());
		act_to_create->SetCollisionSystem(m_collision_system);
		act_to_create->SetECS(m_EntityComponentSystemManager);


		auto& transformComponent = m_EntityComponentSystemManager->GetComponent<TransformComponent>(act_to_copy->GetEntityID());
        auto& meshComponent = m_EntityComponentSystemManager->GetComponent<MeshComponent>(act_to_copy->GetEntityID());
		auto& physComponent = m_EntityComponentSystemManager->GetComponent<PhysicsComponent>(act_to_copy->GetEntityID());
		auto& bboxComponent = m_EntityComponentSystemManager->GetComponent<BoundingBox>(act_to_copy->GetEntityID());


		//ADDING THE TRANSFORM COMPONENT
		m_EntityComponentSystemManager->AddComponent(act_to_create->GetEntityID(), transformComponent);

		//ADDING THE MESH COMPONENT

		m_EntityComponentSystemManager->AddComponent(act_to_create->GetEntityID(), meshComponent);

		//ADDING PHYSICS
		//PhysicsComponent phys;
		m_EntityComponentSystemManager->AddComponent(act_to_create->GetEntityID(), physComponent);

		//ADDING BOUNDINGBOX
		BoundingBox BBox= bboxComponent;
		BBox.SetOwner(act_to_create);
		m_EntityComponentSystemManager->AddComponent(act_to_create->GetEntityID(), BBox);

		m_Max_Actors++;
		int index = m_Max_Actors - 1;
		m_Actors.resize(m_Max_Actors);
		if (m_Max_Actors != 0)
		{
			m_Actors[index] = act_to_create;
			m_Entity_Index[act_to_create->GetEntityID()] = index;
		}
		m_scene_object_manager->AddObject(act_to_create);


	}

	void deleteActorFromManager(Actor* act)
	{

		//Replace the deleted object with the last element of the vector
		Entity m_last_obj_added;
		m_last_obj_added = m_Actors[int(double(m_Max_Actors)-1)]->GetEntityID();
		

		m_Actors[m_Entity_Index[act->GetEntityID()]] = m_Actors[int(double(m_Max_Actors) - 1)];
		m_Entity_Index[m_last_obj_added] = m_Entity_Index[act->GetEntityID()];

		m_Actors[int(double(m_Max_Actors) - 1)] = nullptr;
		m_Entity_Index[act->GetEntityID()] = 0;
		


		m_Max_Actors--;
		m_Actors.resize(m_Max_Actors);
	
		m_scene_object_manager->deleteObjectFromManager(act);

	
	}

	void UpdateTransform(float DeltaTime)
	{

		if (m_Max_Actors != 0)
		for (auto const& entity : m_Entities)
		{
		
			//Get actor components from the ECS
			auto& transform = m_EntityComponentSystemManager->GetComponent<TransformComponent>(entity);
			auto& physics = m_EntityComponentSystemManager->GetComponent<PhysicsComponent>(entity);
	        auto& boundingbox = m_EntityComponentSystemManager->GetComponent<BoundingBox>(entity);

			//Get the actor index from the actor list based on the entity ID
			int index = m_Entity_Index[entity];

			//Temporary variables to add after the bb is updated
			Vector3D temp_push = boundingbox.push_direction;// push direction is set inside the collision system and it can be either -1,0 or 1 on each axis 
			                                 //     ^^^describes from which direction it has collided with an other bounding box
			bool col_enab = boundingbox.collision_enabled;
			//Funtion returns a bb that has the boundaries updated according to the position, size and offset 
			boundingbox = UpdateBoundingBox(transform.position, m_Actors[index]->GetBoundingBoxSize().m_x, m_Actors[index]->GetBoundingBoxSize().m_y, m_Actors[index]->GetBoundingBoxSize().m_z, boundingbox.offset);
		    // restore the data after bounding box is updated
			boundingbox.push_direction = temp_push;
			boundingbox.collision_enabled = col_enab;
			if (m_Actors[index])//if not a nullptr
			{
				
				if (m_Actors[index]->GetGravity())// if actor has gravity enabled 
				{
					//Computing the collision direction  only if the gravitational acceleration !=0
					if (physics.gravitational_acceleration.m_x)
					temp_push.m_x = physics.gravitational_acceleration.m_x / abs(physics.gravitational_acceleration.m_x) - boundingbox.push_direction.m_x;
					//getting the sign of the gravitational acceleration on each axis then subtracting the push direction
					//temp_push should either cancel out or double based on the directions of gravity and collision push
					if(physics.gravitational_acceleration.m_y)
					temp_push.m_y = physics.gravitational_acceleration.m_y / abs(physics.gravitational_acceleration.m_y)- boundingbox.push_direction.m_y;
					if (physics.gravitational_acceleration.m_z)
					temp_push.m_z = physics.gravitational_acceleration.m_z / abs(physics.gravitational_acceleration.m_z) - boundingbox.push_direction.m_z;
					/////////////////////////////////////

					//Created to allow jumping, as the jumping function sets velocity to 30 on the y axis and  b_is_gorunded to false
					if (!m_Actors[index]->GetIsGrounded())temp_push.m_y = -1; // if the actor is not grounded, the velocity is different than 0

                    ////////Calculate velocity caused by the physics component on all axes 
					//add the g acceleration to the velocity and multiplied by the mass of the actor
					//if temp push is 0(meaning the collision direction and g acceleration are opposite), the velocity becomes 0
					physics.velocity.m_x = (physics.velocity.m_x + physics.gravitational_acceleration.m_x * DeltaTime * m_Actors[index]->GetMass()) *-temp_push.m_x;
					physics.velocity.m_y = (physics.velocity.m_y + physics.gravitational_acceleration.m_y * DeltaTime * m_Actors[index]->GetMass()) *-temp_push.m_y;
					physics.velocity.m_z = (physics.velocity.m_z + physics.gravitational_acceleration.m_z * DeltaTime * m_Actors[index]->GetMass()) *-temp_push.m_z;
					//////////////////////////////////

					//Add the velocity to the position of the actor, multiplied by delta time to make it frame rate independent
					transform.position = transform.position + physics.velocity * DeltaTime;

				}


				if (m_Actors[index]->GetSocketAttachment())
				{
					
				    m_Actors[index]->GetSocketAttachment()->UpdateSocket(DeltaTime);
					transform.position = m_Actors[index]->GetSocketAttachment()->GetPosition();
					transform.rotation = m_Actors[index]->GetSocketAttachment()->GetRotation();

				}
				
				if (boundingbox.push_direction.m_y < 0)m_Actors[index]->SetIsGrounded(true);
				else m_Actors[index]->SetIsGrounded(false);
		
			
			}
		}
		

	}

	BoundingBox UpdateBoundingBox(Vector3D center, float sizeX, float sizeY, float sizeZ,Vector3D offset)
	{


		BoundingBox BB;


		BB.center = center + offset;
		BB.offset = offset;
		BB.sizeX = sizeX;
		BB.sizeY = sizeY;
		BB.sizeZ = sizeZ;

	/*	BB.Max_x = center.m_x + offset.m_x + sizeX / 2;
		BB.Min_x = center.m_x + offset.m_x - sizeX / 2;
		BB.Max_y = center.m_y + offset.m_y + sizeY / 2;
		BB.Min_y = center.m_y + offset.m_y - sizeY / 2;
		BB.Max_z = center.m_z + offset.m_z + sizeZ / 2;
		BB.Min_z = center.m_z + offset.m_z - sizeZ / 2;*/
		BB.Max_x = BB.center.m_x + sizeX / 2;
		BB.Min_x = BB.center.m_x - sizeX / 2;
		BB.Max_y = BB.center.m_y + sizeY / 2;
		BB.Min_y = BB.center.m_y - sizeY / 2;
		BB.Max_z = BB.center.m_z + sizeZ / 2;
		BB.Min_z = BB.center.m_z - sizeZ / 2;

		return BB;
	}
	void AddHealthComponent(Entity ID)
	{
		HealthComponent health{};
		m_EntityComponentSystemManager->AddComponent(ID, health);
		int index = m_Entity_Index[ID];
		m_Actors[index]->SetHasHealth(true);

	}

	void AddTexture(Entity ID, const wchar_t* texture_file)
	{
		auto& mesh = m_EntityComponentSystemManager->GetComponent<MeshComponent>(ID);
		mesh.material->addTexture(GraphicsEngine::GetGraphicsEngine()->getTextureManager()->createTextureFromFile(texture_file));

	}
	
	void SetPosition(Entity ID,Vector3D position)
	{
		auto& transform = m_EntityComponentSystemManager->GetComponent<TransformComponent>(ID);
		auto& boundingbox = m_EntityComponentSystemManager->GetComponent<BoundingBox>(ID);
		int index = m_Entity_Index[ID];

		transform.position = position;

		Vector3D temp_push = boundingbox.push_direction;
		bool col_enab = boundingbox.collision_enabled;
		Vector3D temp_prev_loc = transform.position;
		boundingbox = UpdateBoundingBox(transform.position, m_Actors[index]->GetBoundingBoxSize().m_x, m_Actors[index]->GetBoundingBoxSize().m_y, m_Actors[index]->GetBoundingBoxSize().m_z, boundingbox.offset);
		boundingbox.push_direction = temp_push;
		boundingbox.collision_enabled = col_enab;
	}

	Vector3D GetPosition(Entity ID)
	{
		auto& transform = m_EntityComponentSystemManager->GetComponent<TransformComponent>(ID);
		return transform.position;
	}
	void SetScale(Entity ID, Vector3D scale)
	{
		auto& transform = m_EntityComponentSystemManager->GetComponent<TransformComponent>(ID);
		transform.scale = scale;

	}
	void SetRotation(Entity ID, Rotator3D rotation)
	{
		auto& transform = m_EntityComponentSystemManager->GetComponent<TransformComponent>(ID);
		transform.rotation = rotation;
	}
	void SetBoundingBoxOfffset(Entity ID, Vector3D offset)
	{
	
		auto& boundingbox = m_EntityComponentSystemManager->GetComponent<BoundingBox>(ID);
		int index = m_Entity_Index[ID];

		boundingbox.offset = offset;
	
	}


	TransformComponent GetTransform(Entity ID)
	{
		auto& transform = m_EntityComponentSystemManager->GetComponent<TransformComponent>(ID);
		return transform;

	}
	Actor* GetActor(Entity ID)
	{
		int index = m_Entity_Index[ID];
		return m_Actors[index];
	}


private:

	int m_Max_Actors = 0;
	std::vector<Actor*> m_Actors{};
	std::vector<int> m_Entity_Index{};
	EntityComponentSystemManager* m_EntityComponentSystemManager = nullptr;
	std::shared_ptr<SceneObjectManager> m_scene_object_manager{};
	std::shared_ptr<CollisionDetectionSystem> m_collision_system{};

    void AddActor(Actor* act)
	{
		m_Max_Actors++;
		int index = m_Max_Actors - 1;
		m_Actors.resize(m_Max_Actors);
		if (m_Max_Actors != 0)
		{
			m_Actors[index] = act;
			m_Entity_Index[act->GetEntityID()] = index;
		}
		m_scene_object_manager->AddObject(act);
	}
	



	friend class ScenePawnManager;
	friend class SceneCharacterManager;


};