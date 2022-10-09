#pragma once
#include "EntityComponentSystemManager.h"
#include "SceneActorManager.h"
#include "System.h"
#include "Vector3D.h"
#include "Vector2D.h"
#include "Rotator3D.h"
#include "Pawn.h"
#include <iostream>

//extern EntityComponentSystemManager m_EntityComponentSystemManager;

class ScenePawnManager : public SceneActorManager
{
public:
	ScenePawnManager()
	{
		m_Entity_Index.resize(MAX_ENTITIES);
	}
	~ScenePawnManager()
	{

	}
	void Tick(float DeltaTime)
	{
		
		for (auto pawns : m_Pawns)
		{
			pawns->Tick(DeltaTime);
		}
	
	}
	void BeginPlay()
	{
		
		for (auto pawns : m_Pawns)
		{
			pawns->BeginPlay();
		}
		

	}
    void setUp(EntityComponentSystemManager* ECS , std::shared_ptr<SceneActorManager> scene_actor_manager)
	{
		m_EntityComponentSystemManager = ECS;
		m_scene_actor_manager = scene_actor_manager;

	}

	void registerObject(Pawn* pwn, const char* Name, const wchar_t* texture_file, const wchar_t* normal_texture_file, const wchar_t* specular_texture_file,
		const wchar_t* mesh_file, const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path, Vector3D BBsize, CULL_MODE cull_mode
		, int nothing)
	{
		pwn->SetEntityID(m_EntityComponentSystemManager->CreateEntity());
		pwn->SetName(Name);
		pwn->SetBoundingBoxSize(BBsize);
		pwn->SetCollisionSystem(m_collision_system);
		pwn->SetECS(m_EntityComponentSystemManager);

		//ADDING THE TRANSFORM COMPONENT
		TransformComponent tr;
		tr.position = Vector3D(1, 1, 1);
		tr.rotation = Rotator3D(0, 0, 0);
		tr.scale = Vector3D(1, 1, 1);
		m_EntityComponentSystemManager->AddComponent(pwn->GetEntityID(), tr);


		//ADDING THE MESH COMPONENT
		TexturePtr temp_texture = GraphicsEngine::GetGraphicsEngine()->getTextureManager()->createTextureFromFile(texture_file);
		MeshComponent meshComponent;
		meshComponent.mesh = GraphicsEngine::GetGraphicsEngine()->getMeshManager()->createMeshFromFile(mesh_file);
		meshComponent.material = GraphicsEngine::GetGraphicsEngine()->createMaterial(vertex_shader_path, pixel_shader_path);
		temp_texture = GraphicsEngine::GetGraphicsEngine()->getTextureManager()->createTextureFromFile(texture_file);
		meshComponent.material->addTexture(temp_texture);
		if (normal_texture_file)
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


		m_EntityComponentSystemManager->AddComponent(pwn->GetEntityID(), meshComponent);


		//ADDING PHYSICS
		PhysicsComponent phys{};
		m_EntityComponentSystemManager->AddComponent(pwn->GetEntityID(), phys);

		//ADDING BOUNDINGBOX
		BoundingBox BBox{};
		BBox.SetOwner(pwn);
		m_EntityComponentSystemManager->AddComponent(pwn->GetEntityID(), BBox);

		//ADDING CONTROLLER COMPONENT
		ControllerComponent controller;
		controller.position = Vector3D(0, 0, 0);
		controller.rotation = Rotator3D(0, 0, 0);
		m_EntityComponentSystemManager->AddComponent(pwn->GetEntityID(), controller);

		m_Max_Pawns++;
		int index = m_Max_Pawns - 1;
		m_Pawns.resize(m_Max_Pawns);
		if (m_Max_Pawns != 0)
		{
			m_Pawns[index] = pwn;
			m_Entity_Index[pwn->GetEntityID()] = index;

		}
		m_scene_actor_manager->AddActor(pwn);
		
	}
	void deletePawnFromManager(Pawn* pwn)
	{

		//Replace the deleted object with the last element of the vector
		Entity m_last_obj_added;
		m_last_obj_added = m_Pawns[int(double(m_Max_Pawns) - 1)]->GetEntityID();


		m_Pawns[m_Entity_Index[pwn->GetEntityID()]] = m_Pawns[int(double(m_Max_Pawns) - 1)];
		m_Entity_Index[m_last_obj_added] = m_Entity_Index[pwn->GetEntityID()];

		m_Pawns[int(double(m_Max_Pawns) - 1)] = nullptr;
		m_Entity_Index[pwn->GetEntityID()] = 0;
	
		m_Max_Pawns--;
		m_Pawns.resize(m_Max_Pawns);

		m_scene_actor_manager->deleteActorFromManager(pwn);
		
	}
	void UpdateTransformFromInput(float DeltaTime)
	{
		if(m_Max_Pawns>0)
		for (auto const& entity : m_Entities)
		{
			
			auto& transform = m_EntityComponentSystemManager->GetComponent<TransformComponent>(entity);
			auto& boundingbox = m_EntityComponentSystemManager->GetComponent<BoundingBox>(entity);
			auto& controller = m_EntityComponentSystemManager->GetComponent<ControllerComponent>(entity);

            if (!controller.receive_input)return;

			
			Matrix4x4 world_model, temp;
			world_model.setIdentity();

			//std::cout<< controller.delta_mouse_x<<"  " << controller.delta_mouse_y<<std::endl;
			controller.rotation.m_x = controller.delta_mouse_y * DeltaTime * controller.mouse_sensitivity;
			controller.rotation.m_y = controller.delta_mouse_x * DeltaTime * controller.mouse_sensitivity;

			/*if (controller.rotation.m_x >= 90)controller.rotation.m_x = 90;
			else if (controller.rotation.m_x <= -90)controller.rotation.m_x = -90;*/

	        transform.rotation = transform.rotation + controller.rotation;
		

			if(m_Pawns[m_Entity_Index[entity]]->GetCanFly())
			{ 
            temp.setIdentity();
			temp.setRotationRadiansX(transform.rotation.GetRadianValue().m_x);
			world_model *= temp;
            }
			else transform.rotation.m_x = 0;

			temp.setIdentity();
			temp.setRotationRadiansY(transform.rotation.GetRadianValue().m_y);
			world_model *= temp;

			//std::cout << controller.m_forward_axis;
			if(controller.m_forward_axis || controller.m_rightward_axis)
			{ 
			controller.position =  world_model.getZDirection() * (controller.m_forward_axis) * 50 * DeltaTime
				                                       - world_model.getXDirection() * (controller.m_rightward_axis) * 50 * DeltaTime;
             
		    transform.position = transform.position + controller.position;

		//	transform.position = Vector3D::lerp(transform.position, transform.position+ controller.position, 6.0f * DeltaTime);

            }

        
		
		   int index = m_Entity_Index[entity];

		   boundingbox = UpdateBoundingBox(transform.position, m_Pawns[index]->GetBoundingBoxSize().m_x, m_Pawns[index]->GetBoundingBoxSize().m_y, m_Pawns[index]->GetBoundingBoxSize().m_z, boundingbox.offset);
		
		}
	
		
	}
	
	TransformComponent GetTransform(Entity ID)
	{
		auto& transform = m_EntityComponentSystemManager->GetComponent<TransformComponent>(ID);
		return transform;

	}

private:

	int m_Max_Pawns = 0;
	
	std::vector<Pawn*> m_Pawns{};

	std::vector<int> m_Entity_Index{};
	EntityComponentSystemManager* m_EntityComponentSystemManager = nullptr;
	std::shared_ptr<SceneActorManager> m_scene_actor_manager{};

	void AddPawn(Pawn* pwn)
	{
		m_Max_Pawns++;
		int index = m_Max_Pawns - 1;
		m_Pawns.resize(m_Max_Pawns);
		if (m_Max_Pawns != 0)
		{
			m_Pawns[index] = pwn;
			m_Entity_Index[pwn->GetEntityID()] = index;
		}
		m_scene_actor_manager->AddActor(pwn);
	}


	friend class SceneCharacterManager;

};
