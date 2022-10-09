#pragma once
#include "EntityComponentSystemManager.h"
#include "ScenePawnManager.h"
#include "System.h"
#include "Vector3D.h"
#include "Vector2D.h"
#include "Rotator3D.h"
#include "Character.h"
#include <iostream>


//extern EntityComponentSystemManager m_EntityComponentSystemManager;

class SceneCharacterManager : public ScenePawnManager
{
public:
	SceneCharacterManager()
	{
		m_Entity_Index.resize(MAX_ENTITIES);
	}
	~SceneCharacterManager()
	{
		
	}
	void Tick(float DeltaTime)
	{
		
		for (auto characters : m_Characters)
		{
			characters->Tick(DeltaTime);
		
		}
		

	}
	void BeginPlay()
	{
		
		for (auto characters : m_Characters)
		{
			characters->BeginPlay();
		}


	}
	void setUp(EntityComponentSystemManager* ECS, std::shared_ptr<ScenePawnManager> scene_pawn_manager)
	{
		m_EntityComponentSystemManager = ECS;
		m_scene_pawn_manager = scene_pawn_manager;
	}

	void registerObject(Character*cht, const char* Name, const wchar_t* texture_file, const wchar_t* normal_texture_file, const wchar_t* specular_texture_file,
		const wchar_t* mesh_file, const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path,
		Vector3D BBsize, CULL_MODE cull_mode,
		Vector3D camera_offset_position, Rotator3D camera_rotation, Vector3D camera_scale, Vector3D relative_pos_to_cht,
		float FOV, float aspect, float near_plane, float far_plane)
	{
		
		cht->SetEntityID(m_EntityComponentSystemManager->CreateEntity());
		cht->SetName(Name);
		cht->SetBoundingBoxSize(BBsize);
		cht->SetCollisionSystem(m_collision_system);
		cht->SetECS(m_EntityComponentSystemManager);


		//ADDING THE TRANSFORM COMPONENT
		TransformComponent tr;
		tr.position = Vector3D(1, 1, 1);
		tr.rotation = Rotator3D(0, 0, 0);
		tr.scale = Vector3D(1, 1, 1);
		m_EntityComponentSystemManager->AddComponent(cht->GetEntityID(), tr);

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



		m_EntityComponentSystemManager->AddComponent(cht->GetEntityID(), meshComponent);


		//ADDING PHYSICS
		PhysicsComponent phys{};
		m_EntityComponentSystemManager->AddComponent(cht->GetEntityID(), phys);

		//ADDING BOUNDINGBOX
		BoundingBox BBox{};
		BBox.SetOwner(cht);
		m_EntityComponentSystemManager->AddComponent(cht->GetEntityID(), BBox);

		//ADDING CONTROLLER COMPONENT
		ControllerComponent controller;
		controller.position = Vector3D(0, 0, 0);
		controller.rotation = Rotator3D(0, 0, 0);
		m_EntityComponentSystemManager->AddComponent(cht->GetEntityID(), controller);

		//ADDING CAMERA COMPONENT
		CameraComponent camera;
		camera.position = camera_offset_position + relative_pos_to_cht;
		camera.rotation = camera_rotation;
		camera.scale = camera_scale;
		camera.offset_pos = camera_offset_position;
		camera.relative_pos_to_cht = relative_pos_to_cht;
		camera.FOV = FOV * float(3.14 / 180); //converting from degrees to radians
		camera.aspect = aspect;
		camera.near_plane = near_plane;
		camera.far_plane = far_plane;

		m_EntityComponentSystemManager->AddComponent(cht->GetEntityID(), camera);


		m_Max_Characters++;
		int index = m_Max_Characters - 1;
		m_Characters.resize(m_Max_Characters);
		if (m_Max_Characters != 0)
		{
			m_Characters[index] = cht;
			m_Entity_Index[cht->GetEntityID()] = index;
		}
		m_scene_pawn_manager->AddPawn(cht);
		
	
	}
	void deleteCharacterFromManager(Character* cht)
	{

		//Replace the deleted object with the last element of the vector
		Entity m_last_obj_added;
		m_last_obj_added = m_Characters[int(double(m_Max_Characters) - 1)]->GetEntityID();


		m_Characters[m_Entity_Index[cht->GetEntityID()]] = m_Characters[int(double(m_Max_Characters) - 1)];
		m_Entity_Index[m_last_obj_added] = m_Entity_Index[cht->GetEntityID()];

		m_Characters[int(double(m_Max_Characters) - 1)] = nullptr;
		m_Entity_Index[cht->GetEntityID()] = 0;
		
		m_Max_Characters--;
		m_Characters.resize(m_Max_Characters);

		m_scene_pawn_manager->deletePawnFromManager(cht);

	
	}
	


	void UpdateTransformFromInput(float DeltaTime)
	{
		
		for (auto const& entity : m_Entities)
		{
			

			auto& transform = m_EntityComponentSystemManager->GetComponent<TransformComponent>(entity);
			auto& boundingbox = m_EntityComponentSystemManager->GetComponent<BoundingBox>(entity);
			auto& controller = m_EntityComponentSystemManager->GetComponent<ControllerComponent>(entity);
            auto& camera = m_EntityComponentSystemManager->GetComponent<CameraComponent>(entity);

			if (!controller.receive_input)return;

			Matrix4x4 world_cam, temp;
			world_cam.setIdentity();

		
			camera.rotation.m_x += controller.delta_mouse_y * DeltaTime * controller.mouse_sensitivity;
			camera.rotation.m_y += controller.delta_mouse_x * DeltaTime * controller.mouse_sensitivity;

			if (camera.rotation.m_x >= 90)camera.rotation.m_x = 90;
			else if (camera.rotation.m_x <= -90)camera.rotation.m_x = -90;
			
			//camera.rotation = Rotator3D::lerpRotator(camera.rotation, tempRot, 10.0f * DeltaTime);

            temp.setIdentity();
			temp.setRotationRadiansX(camera.rotation.GetRadianValue().m_x);
			world_cam *= temp;

			temp.setIdentity();
			temp.setRotationRadiansY(camera.rotation.GetRadianValue().m_y);
			world_cam *= temp;


			Vector3D tempPos;
			tempPos = transform.position + world_cam.getZDirection() * camera.relative_pos_to_cht.m_z
				                         + world_cam.getYDirection() * camera.relative_pos_to_cht.m_y
				                         + world_cam.getXDirection() * camera.relative_pos_to_cht.m_x
				                         + camera.offset_pos;

			camera.position = tempPos;

			int index = m_Entity_Index[entity];	
			boundingbox = UpdateBoundingBox(transform.position, m_Characters[index]->GetBoundingBoxSize().m_x, m_Characters[index]->GetBoundingBoxSize().m_y, m_Characters[index]->GetBoundingBoxSize().m_z, boundingbox.offset);
		
		}

		
	}
	
	TransformComponent GetTransform(Entity ID)
	{
		auto& transform = m_EntityComponentSystemManager->GetComponent<TransformComponent>(ID);
		return transform;

	}

private:
	
	int m_Max_Characters = 0;

	
	std::vector<Character*> m_Characters{};

	std::vector<int> m_Entity_Index{};
	EntityComponentSystemManager* m_EntityComponentSystemManager = nullptr;
	std::shared_ptr<ScenePawnManager> m_scene_pawn_manager{};

};
