#include "Level.h"

Levels Level::m_active_level = Levels::EMainMenu;
bool Level::b_change_level = false;
Level::Level()
{
		
	setUpObjectData(); 
	b_has_objects = true;
	b_change_level = false;
}

void Level::setUpObjectData()
{
	m_EntityComponentSystemManager->Init();
	////3D SCENE SYSTEMS
	m_EntityComponentSystemManager->RegisterComponent<TransformComponent>();
	m_EntityComponentSystemManager->RegisterComponent<MeshComponent>();
	m_EntityComponentSystemManager->RegisterComponent<BoundingBox>();
	m_EntityComponentSystemManager->RegisterComponent<ControllerComponent>();
	m_EntityComponentSystemManager->RegisterComponent<CameraComponent>();
	m_EntityComponentSystemManager->RegisterComponent<PhysicsComponent>();
	m_EntityComponentSystemManager->RegisterComponent<HealthComponent>();


	m_objectManager = m_EntityComponentSystemManager->RegisterSystem<SceneObjectManager>();
	m_objectManager->setUp(m_EntityComponentSystemManager);
	m_actorManager = m_EntityComponentSystemManager->RegisterSystem<SceneActorManager>();
	m_actorManager->setUp(m_EntityComponentSystemManager, m_objectManager);
	m_pawnManager = m_EntityComponentSystemManager->RegisterSystem<ScenePawnManager>();
	m_pawnManager->setUp(m_EntityComponentSystemManager, m_actorManager);
	m_characterManager = m_EntityComponentSystemManager->RegisterSystem<SceneCharacterManager>();
	m_characterManager->setUp(m_EntityComponentSystemManager, m_pawnManager);
	m_collision_system = m_EntityComponentSystemManager->RegisterSystem<CollisionDetectionSystem>() ;
	m_collision_system->setECS(m_EntityComponentSystemManager);

	Signature signature_object;

	Signature signature_actor;
	signature_actor.set(m_EntityComponentSystemManager->GetComponentType<TransformComponent>());
	signature_actor.set(m_EntityComponentSystemManager->GetComponentType<MeshComponent>());
	signature_actor.set(m_EntityComponentSystemManager->GetComponentType<PhysicsComponent>());
	signature_actor.set(m_EntityComponentSystemManager->GetComponentType<BoundingBox>());
	m_EntityComponentSystemManager->SetSystemSignature<SceneActorManager>(signature_actor);

	Signature signature_pawn;
	signature_pawn.set(m_EntityComponentSystemManager->GetComponentType<TransformComponent>());
	signature_pawn.set(m_EntityComponentSystemManager->GetComponentType<MeshComponent>());
	signature_pawn.set(m_EntityComponentSystemManager->GetComponentType<ControllerComponent>());
	signature_pawn.set(m_EntityComponentSystemManager->GetComponentType<BoundingBox>());
	m_EntityComponentSystemManager->SetSystemSignature<ScenePawnManager>(signature_pawn);

	Signature signature_character;
	signature_character.set(m_EntityComponentSystemManager->GetComponentType<TransformComponent>());
	signature_character.set(m_EntityComponentSystemManager->GetComponentType<MeshComponent>());
	signature_character.set(m_EntityComponentSystemManager->GetComponentType<ControllerComponent>());
	signature_character.set(m_EntityComponentSystemManager->GetComponentType<CameraComponent>());
	signature_character.set(m_EntityComponentSystemManager->GetComponentType<BoundingBox>());
	m_EntityComponentSystemManager->SetSystemSignature<SceneCharacterManager>(signature_character);

	Signature signature_collision_system;
	signature_collision_system.set(m_EntityComponentSystemManager->GetComponentType<BoundingBox>());
	m_EntityComponentSystemManager->SetSystemSignature<CollisionDetectionSystem>(signature_collision_system);


	m_actorManager->setCollisionSystem(m_collision_system);
	m_pawnManager->setCollisionSystem(m_collision_system);
	m_characterManager->setCollisionSystem(m_collision_system);

	//2D SCENE SYSTEMS

	m_EntityComponentSystemManager->RegisterComponent<TransformComponent2D>();

	m_widget_manager = m_EntityComponentSystemManager->RegisterSystem<WidgetManager>();
	m_widget_manager->setECS(m_EntityComponentSystemManager);

	Signature signature_widget;
	signature_widget.set(m_EntityComponentSystemManager->GetComponentType<TransformComponent2D>());
	m_EntityComponentSystemManager->SetSystemSignature<WidgetManager>(signature_widget);

}
void Level::BeginPlay()
{
	m_play_state = true;
	int width = m_window_rect.right - m_window_rect.left;
	int height = m_window_rect.bottom - m_window_rect.top;
	
	m_widget_manager->RegisterText(m_frames_per_second, "FPS", L" fps", Point( width / 8, height / 10), Point(width / 10, height / 10), Vector3D(1, 1, 1), 40);
	m_frames_per_second->UpdateText(0);

	m_objectManager->BeginPlay();
	m_actorManager->BeginPlay();
	m_pawnManager->BeginPlay();
	m_characterManager->BeginPlay();
	m_widget_manager->BeginPlay();

	

}
Level::~Level()
{

}

void Level::Tick(float DeltaTime)
{
	updateLight();
}


void Level::updateActors(float DeltaTime)
{ 
  
  if(b_has_objects)
  { 
	  if (m_play_state)
	  {
		  //UPDATE SOCKETS ATTACHED
		  m_objectManager->Tick(DeltaTime);
		  //FIRST UPDATE THE TRANSFORM FROM THE CONTROLLER 
		  m_pawnManager->Tick(DeltaTime);
		  m_pawnManager->UpdateTransformFromInput(DeltaTime);

		  for (auto const& entity1 : m_characterManager->m_Entities)//Check collisions between characters and actors
		  {
			  auto& bb1 = m_EntityComponentSystemManager->GetComponent<BoundingBox>(entity1);
			  auto& transform = m_EntityComponentSystemManager->GetComponent<TransformComponent>(entity1);
			  for (auto const& entity2 : m_actorManager->m_Entities)
			  {

				  auto& bb2 = m_EntityComponentSystemManager->GetComponent<BoundingBox>(entity2);
				  if(bb2.collision_enabled)
				  if ( entity1 != entity2 && bb2.sizeX != 0)
				  {
					  Vector3D temp_push = bb1.push_direction;

					  CollisionResult CRes = m_collision_system->CheckBBtoBBCollision(bb1, bb2);

					  if (bb1.push_direction.m_x == 0)
						  bb1.push_direction.m_x = temp_push.m_x;
					  if (bb1.push_direction.m_y == 0)
						  bb1.push_direction.m_y = temp_push.m_y;
					  if (bb1.push_direction.m_z == 0)
						  bb1.push_direction.m_z = temp_push.m_z;

					  if (CRes.collided)
					  {
						 
						  transform.position = transform.position + CRes.InsideDelta;
					  }


				  }


			  }
		  }

		  //UPDATE THE ACTORS ATTACHED TO THE SOCKETS
		  m_actorManager->Tick(DeltaTime);
		  m_actorManager->UpdateTransform(DeltaTime);
		  //UPDATE CAMERA
		  m_characterManager->Tick(DeltaTime);
		  m_characterManager->UpdateTransformFromInput(DeltaTime);
		  updateCamera();


	  }
	
	

    constant constantObject;


	constantObject.m_view = m_view_camera;
	constantObject.m_proj = m_proj_camera;
	constantObject.m_camera_position = m_world_camera.getTranslation();

	constantObject.m_light_position = m_light_position;
	constantObject.m_light_radius = 0.0f;
	constantObject.m_light_direction = m_light_rotation_matrix.getZDirection();
	//Draw the meshes with all the new transform
	for (auto const& entity : m_actorManager->m_Entities)
	{
		
		auto& transform = m_EntityComponentSystemManager->GetComponent<TransformComponent>(entity);
		auto& meshComp = m_EntityComponentSystemManager->GetComponent<MeshComponent>(entity);
		auto& bb = m_EntityComponentSystemManager->GetComponent<BoundingBox>(entity);
		
		/*debuggtool->DrawLine(Vector3D(bb.Max_x, bb.Max_y, bb.Max_z), Vector3D(bb.Max_x, bb.Min_y, bb.Max_z), 0.2, Vector3D(1, 0, 0), m_world_camera, m_view_camera, m_proj_camera);
		debuggtool->DrawLine(Vector3D(bb.Min_x, bb.Max_y, bb.Max_z), Vector3D(bb.Min_x, bb.Min_y, bb.Max_z), 0.2, Vector3D(1, 0, 0), m_world_camera, m_view_camera, m_proj_camera);
		debuggtool->DrawLine(Vector3D(bb.Min_x, bb.Max_y, bb.Min_z), Vector3D(bb.Min_x, bb.Min_y, bb.Min_z), 0.2, Vector3D(1, 0, 0), m_world_camera, m_view_camera, m_proj_camera);
		debuggtool->DrawLine(Vector3D(bb.Max_x, bb.Max_y, bb.Min_z), Vector3D(bb.Max_x, bb.Min_y, bb.Min_z), 0.2, Vector3D(1, 0, 0), m_world_camera, m_view_camera, m_proj_camera);

		debuggtool->DrawLine(Vector3D(bb.Max_x, bb.Max_y, bb.Max_z), Vector3D(bb.Min_x, bb.Max_y, bb.Max_z), 0.2, Vector3D(1, 0, 0), m_world_camera, m_view_camera, m_proj_camera);
		debuggtool->DrawLine(Vector3D(bb.Max_x, bb.Min_y, bb.Max_z), Vector3D(bb.Min_x, bb.Min_y, bb.Max_z), 0.2, Vector3D(1, 0, 0), m_world_camera, m_view_camera, m_proj_camera);
		debuggtool->DrawLine(Vector3D(bb.Max_x, bb.Max_y, bb.Min_z), Vector3D(bb.Min_x, bb.Max_y, bb.Min_z), 0.2, Vector3D(1, 0, 0), m_world_camera, m_view_camera, m_proj_camera);
		debuggtool->DrawLine(Vector3D(bb.Max_x, bb.Min_y, bb.Min_z), Vector3D(bb.Min_x, bb.Min_y, bb.Min_z), 0.2, Vector3D(1, 0, 0), m_world_camera, m_view_camera, m_proj_camera);


		debuggtool->DrawLine(Vector3D(bb.Max_x, bb.Max_y, bb.Max_z), Vector3D(bb.Max_x, bb.Max_y, bb.Min_z), 0.2, Vector3D(1, 0, 0), m_world_camera, m_view_camera, m_proj_camera);
		debuggtool->DrawLine(Vector3D(bb.Max_x, bb.Min_y, bb.Max_z), Vector3D(bb.Max_x, bb.Min_y, bb.Min_z), 0.2, Vector3D(1, 0, 0), m_world_camera, m_view_camera, m_proj_camera);
		debuggtool->DrawLine(Vector3D(bb.Min_x, bb.Max_y, bb.Max_z), Vector3D(bb.Min_x, bb.Max_y, bb.Min_z), 0.2, Vector3D(1, 0, 0), m_world_camera, m_view_camera, m_proj_camera);
		debuggtool->DrawLine(Vector3D(bb.Min_x, bb.Min_y, bb.Max_z), Vector3D(bb.Min_x, bb.Min_y, bb.Min_z), 0.2, Vector3D(1, 0, 0), m_world_camera, m_view_camera, m_proj_camera);*/

		if(meshComp.visible)
		{ 
        constantObject.m_alpha = meshComp.texture_alpha;
		m_list_materials.clear();
		m_list_materials.push_back(meshComp.material);
	
	//	transform.position = Vector3D::lerp(transform.position, transform.position, 6.0f * DeltaTime);

		Material::updateModel(transform.position, transform.rotation, transform.scale, m_list_materials, constantObject);

		GraphicsEngine::GetGraphicsEngine()->drawMesh(meshComp.mesh, m_list_materials);
        }


	}
	
  }
  
}

void Level::updateWidgets(float DeltaTime)
{
    m_widget_manager->CheckButtonHoverAndPress(m_cursor_window_position, m_mouse_L_pressed);
    m_widget_manager->Tick(DeltaTime);

	int width = m_window_rect.right - m_window_rect.left;
	int height = m_window_rect.bottom - m_window_rect.top;
	if(DeltaTime)
	m_frames_per_second->UpdateText(int(1.0f / DeltaTime));
	m_frames_per_second->SetPosition(Point(width / 8, height / 10));
}

void Level::SetCameraOwner(Character* camera_owner)
{
	m_camera_owner = camera_owner;
}

void Level::SetActiveLevel(Levels active_level)
{
	m_active_level = active_level;
}

Levels Level::GetActiveLevel()
{
	return m_active_level;
}

void Level::updateLight()
{
	Matrix4x4 temp;

	m_light_rotation_matrix.setIdentity();

	temp.setIdentity();
	temp.setRotationDegreesX(-60);
	m_light_rotation_matrix *= temp;

	temp.setIdentity();
	temp.setRotationDegreesY(115);
	m_light_rotation_matrix *= temp;

}


bool Level::GetChangeLevel()
{
	return b_change_level;
}

void Level::updateCamera()
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentity();
	if(b_has_objects)
	{ 

	if(m_camera_owner)
	{ 
    //Get the camera component from the character that is set as the camera owner
	auto& cameraComp = m_EntityComponentSystemManager->GetComponent<CameraComponent>(m_camera_owner->GetEntityID());
	
	temp.setIdentity();
	temp.setRotationRadiansX(cameraComp.rotation.GetRadianValue().m_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationRadiansY(cameraComp.rotation.GetRadianValue().m_y);
	world_cam *= temp;
	world_cam.setTranslation(cameraComp.position);
	m_world_camera = world_cam;
	world_cam.inverse();
	m_view_camera = world_cam;

	m_proj_camera.setPerspectiveFovLH(cameraComp.FOV, cameraComp.aspect, cameraComp.near_plane, cameraComp.far_plane);
    }
	else
	{

		int width = m_window_rect.right - m_window_rect.left;
		int height = m_window_rect.bottom - m_window_rect.top;
		temp.setIdentity();
		temp.setRotationRadiansX(0);
		world_cam *= temp;

		temp.setIdentity();
		temp.setRotationRadiansY(0);
		world_cam *= temp;

		world_cam.setTranslation(Vector3D(0,0,0));
		m_world_camera = world_cam;
		world_cam.inverse();
		m_view_camera = world_cam;
		m_proj_camera.setPerspectiveFovLH(90, ((float)width / (float)height), 0.1f, 5000.0f);
	}


	}



}

void Level::onKeyDown(int key)
{
	
	
}

void Level::onKeyUp(int key)
{
	

}

void Level::onMouseMove(const Point& mouse_pos)
{

	if (!m_play_state)
	{
		m_cursor_window_position.m_x = mouse_pos.m_x - m_window_rect.left;
		m_cursor_window_position.m_y = mouse_pos.m_y - m_window_rect.top;

		// std::cout << m_cursor_window_position.m_x<<std::endl;
		return;
	}



}

void Level::onLeftMouseDown(const Point& mouse_pos)
{
	if (!m_play_state)
	{
		m_mouse_L_pressed = true;

	}

}

void Level::onLeftMouseUp(const Point& mouse_pos)
{
	m_mouse_L_pressed = false;
}

void Level::onRightMouseDown(const Point& mouse_pos)
{

}

void Level::onRightMouseUp(const Point& mouse_pos)
{

}

bool Level::getPlayState()
{
	return m_play_state;
}

void Level::setPlayState(bool state)
{
	m_play_state = state;
}

void Level::SetWindowRect(RECT rect)
{
	m_window_rect  = rect;
}
