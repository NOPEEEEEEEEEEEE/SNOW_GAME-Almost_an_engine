#include "GameLevel.h"
#include <time.h> 

bool GameLevel::b_restart = false;
Sound* GameLevel::m_ambiental_music = new Sound;
TextWidget* GameLevel::m_loading_text = new TextWidget;
GameLevel::GameLevel()
{
	 //	           |Y
	 //	           |
	 //            |
	 //            |__________ X
	 //           /
	 //        Z /

   m_this_level = Levels::EGameLevel;
   b_restart = false;
   m_play_state = true;
   //calculate the instancing numbers based on density and map size
   m_tree_number = m_tree_density* m_ground_width* m_ground_length;
   m_turret_number = m_turret_density *m_ground_width * m_ground_length;
}
void GameLevel::BeginPlay()
{
	//Calculate window sizes
	int width = m_window_rect.right - m_window_rect.left;
	int height = m_window_rect.bottom - m_window_rect.top;

	// OBJECT REGISTRATIONS--------------------------------------------------------------------------------------------------

	//Sky Box
	m_actorManager->registerObject(m_sky_box, "SkyBox", L"Assets\\Textures\\HDRI\\Snow_mountains_HDRI.jpg", NULL, NULL
		, L"Assets\\Meshes\\sphere.obj", L"SkyBoxVertexShader.hlsl", L"SkyBoxPixelShader.hlsl"
		, Vector3D(0, 0, 0), CULL_MODE::CULL_MODE_FRONT);
	m_actorManager->SetScale(m_sky_box->GetEntityID(), Vector3D(4800, 4800, 4800));
   
	m_sky_box->SetGravity(false);
	//Character---------------------------------------------------------------------------------------------
	m_characterManager->registerObject(m_cht, "Cht", L"Assets\\Textures\\Character_Texture\\Zlorp_Albedo.png",
		L"Assets\\Textures\\Character_Texture\\Zlorp_Normal.png",
		L"Assets\\Textures\\Character_Texture\\Zlorp_Specular.png",
		L"Assets\\Meshes\\Characters\\Zlorp.obj",
		L"Vertex_Shader.hlsl",
		L"Pixel_Shader.hlsl",
		Vector3D(7, 14, 7), CULL_MODE::CULL_MODE_BACK, Vector3D(0, 10, 0), Rotator3D(0, 0, 0), Vector3D(1, 1, 1),
		Vector3D(4, 10, -8), 90, ((float)width / (float)height), 0.1f, 5000.0f);
	m_cht->SetMass(4);
	m_actorManager->SetPosition(m_cht->GetEntityID(), Vector3D(1, 15, 2));
	m_actorManager->SetScale(m_cht->GetEntityID(), Vector3D(10, 10, 10));
	m_actorManager->SetBoundingBoxOfffset(m_cht->GetEntityID(), Vector3D(0, 10, 0));
	m_actorManager->AddHealthComponent(m_cht->GetEntityID());
	//m_cht->SetCanFly(true);
   // m_cht->SetGravity(false);
	SetCameraOwner(m_cht);


	//Gun---------------------------------------------------------------------------------------------
	m_actorManager->registerObject(m_gun, "Gun", L"Assets\\Textures\\Gun_textures\\Gun_albedo.jpg",
		L"Assets\\Textures\\Gun_textures\\Gun_normal.jpg",
		L"Assets\\Textures\\Gun_textures\\Gun_specular.jpg",
		L"Assets\\Meshes\\Weapons\\SnowGun.obj",
		L"Vertex_Shader.hlsl",
		L"Pixel_Shader.hlsl",
		Vector3D(0, 0, 0), CULL_MODE::CULL_MODE_BACK);
	m_actorManager->SetScale(m_gun->GetEntityID(), Vector3D(2.5, 2.5, 2.5));


	m_body_socket = new Socket(m_cht->GetEntityID(), Vector3D(0, 0, 0), Rotator3D(0, 0, 0));
	m_body_socket->SetInheritRotation(false);
	m_hand_socket = new Socket(m_cht->GetEntityID(), Vector3D(1.3f, 14, 3), Rotator3D(0, 0, 0));
	m_gun_socket = new Socket(m_gun->GetEntityID(), Vector3D(0, 1, 5), Rotator3D(0, 0, 0));
    m_sky_box->AttachTo(m_body_socket);
	                     
	m_objectManager->registerObject(m_body_socket, "body_socket");
	m_objectManager->registerObject(m_hand_socket, "hand_socket");
	m_objectManager->registerObject(m_gun_socket, "muzzle_socket");
	   
	m_gun->AttachTo(m_hand_socket);
	m_gun->SetGravity(false);


	//GROUND---------------------------------------------------------------------------------------------

	m_ground_tiles.resize(long long int(m_ground_width)* long long int(m_ground_length));
	m_ground_tiles[0] = new Actor;
	m_actorManager->registerObject(m_ground_tiles[0], "Ground", L"Assets\\Textures\\Snow_texture_1\\Snow_Albedo.jpg"
		, L"Assets\\Textures\\Snow_texture_1\\Snow_Normal.jpg"
		, L"Assets\\Textures\\Snow_texture_1\\SN_2\\Snow_specular.jpg"
		, L"Assets\\Meshes\\plane1.obj"
		, L"Snow_Ground_VertexShader.hlsl"
		, L"Snow_Ground_PixelShader.hlsl", Vector3D(2* m_ground_length * m_ground_tile_size, 5, 2*m_ground_width* m_ground_tile_size), CULL_MODE::CULL_MODE_BACK);

    m_actorManager->AddTexture(m_ground_tiles[0]->GetEntityID(), L"Assets\\Textures\\Rock_Ground\\V2\\Albedo.jpg");
	m_actorManager->AddTexture(m_ground_tiles[0]->GetEntityID(), L"Assets\\Textures\\Rock_Ground\\V2\\Normal.jpg");
	m_actorManager->AddTexture(m_ground_tiles[0]->GetEntityID(), L"Assets\\Textures\\Rock_Ground\\V2\\Specular.jpg");
	m_actorManager->AddTexture(m_ground_tiles[0]->GetEntityID(), L"Assets\\Textures\\Rock_Ground\\V2\\Height.jpg");

	m_actorManager->SetScale(m_ground_tiles[0]->GetEntityID(), Vector3D(m_ground_tile_size, 1, m_ground_tile_size));

	m_actorManager->SetPosition(m_ground_tiles[0]->GetEntityID(),
		Vector3D(-m_ground_tile_size*(m_ground_length -1),0, -m_ground_tile_size* (m_ground_width - 1))); 
	
	m_actorManager->SetBoundingBoxOfffset(m_ground_tiles[0]->GetEntityID(),
		Vector3D(m_ground_tile_size * (m_ground_length - 1), 0, m_ground_tile_size * (m_ground_width - 1)));

	m_ground_tiles[0]->SetGravity(false);
	

	for (int i = 0; i < m_ground_length; i++)
	{
		for (int j = 0; j < m_ground_width; j++)
		{
			if (!(i == 0 && j == 0))
			{ 

			m_ground_tiles[long long int(j)+ long long int(m_ground_width) * long long int(i)] = new Actor();
			m_actorManager->InstantiateActor(m_ground_tiles[long long int(j) + long long int(m_ground_width) * long long int(i)], m_ground_tiles[0]);

			m_actorManager->SetPosition(m_ground_tiles[long long int(j) + long long int(m_ground_width) * long long int(i)]->GetEntityID(),
			m_ground_tiles[0]->GetPosition()+Vector3D( float(i* m_ground_tile_size*2), 0, float(j * m_ground_tile_size*2) ) );
	
			m_ground_tiles[long long int(j) + long long int(m_ground_width) * long long int(i)]->SetGravity(false);
			m_ground_tiles[long long int(j) + long long int(m_ground_width) * long long int(i)]->SetCollision(false);
			
            }
		}

	}
	//TREES---------------------------------------------------------------------------------------------
	m_trees.resize(m_tree_number);

	srand(unsigned int(time(NULL)));

	m_trees[0] = new Actor();
	

           m_actorManager->registerObject(m_trees[0], "Tree", L"Assets\\Textures\\Trees\\Fir_v1.jpg"
				, L"Assets\\Textures\\Snow_texture_1\\Snow_Normal.jpg"
				, L"Assets\\Textures\\Snow_texture_1\\SN_2\\Snow_specular.jpg"
				, L"Assets\\Meshes\\Pine1.obj"
				, L"Vertex_Shader.hlsl"
				, L"Pixel_Shader.hlsl", Vector3D(20, 65, 20), CULL_MODE::CULL_MODE_NONE);
			m_actorManager->SetPosition(m_trees[0]->GetEntityID(), Vector3D(m_ground_length * m_ground_tile_size - 50, 0, m_ground_width * m_ground_tile_size - 30));
			m_actorManager->SetScale(m_trees[0]->GetEntityID(), Vector3D(10, 10, 10));
			m_actorManager->SetBoundingBoxOfffset(m_trees[0]->GetEntityID(), Vector3D(0, 35, 0));
			m_trees[0]->SetGravity(false);

    m_trees[1] = new Actor();

			m_actorManager->registerObject(m_trees[1], "Tree2", L"Assets\\Textures\\Trees\\Pine_v1.jpg"
				, L"Assets\\Textures\\Snow_texture_1\\Snow_Normal.jpg"
				, L"Assets\\Textures\\Snow_texture_1\\SN_2\\Snow_specular.jpg"
				, L"Assets\\Meshes\\Pine2.obj"
				, L"Vertex_Shader.hlsl"
				, L"Pixel_Shader.hlsl", Vector3D(30, 60, 30), CULL_MODE::CULL_MODE_NONE);
			m_actorManager->SetPosition(m_trees[1]->GetEntityID(), Vector3D(m_ground_length * m_ground_tile_size - 50, 0, -m_ground_width * m_ground_tile_size + 40));
			m_actorManager->SetScale(m_trees[1]->GetEntityID(), Vector3D(10, 10, 10));
			m_actorManager->SetBoundingBoxOfffset(m_trees[1]->GetEntityID(), Vector3D(0, 30, 0));
			m_trees[1]->SetGravity(false);

                                    
	for (int i = 2; i < m_tree_number; i++) //generate tree positions create them and choose randomly between two tree models
	{
		int pos_randz;
		int signz;
		int pos_randx;
		int signx;
		int tree_type;

		pos_randz = rand() %int(m_ground_width* m_ground_tile_size - 40) + 30;
		signz = rand() % 2 + 0;
		if (signz == 0)pos_randz = -pos_randz;

		pos_randx = rand() % int(m_ground_length *m_ground_tile_size - 40) + 30;
		signx = rand() % 2 + 0;
		if (signx == 0)pos_randx = -pos_randx;

		tree_type = rand() % 2 + 0;
		if (tree_type == 0)
		{
	
			m_trees[i] = new Actor();
			m_actorManager->InstantiateActor(m_trees[i], m_trees[0]);

			m_actorManager->SetPosition(m_trees[i]->GetEntityID(), Vector3D(float(pos_randx), 0, float(pos_randz)));
			m_trees[i]->SetGravity(false);

		}
		else
		{
			m_trees[i] = new Actor(); 
			m_actorManager->InstantiateActor(m_trees[i], m_trees[1]);

			m_actorManager->SetPosition(m_trees[i]->GetEntityID(), Vector3D(float(pos_randx), 0, float(pos_randz)));
			m_trees[i]->SetGravity(false);

		}
	}
	//WALLS---------------------------------------------------------------------------------------------
   
	m_walls.resize(2);
	    //first orientation
	m_walls[0] = new Actor();
			m_actorManager->registerObject(m_walls[0], "Wall", L"Assets\\Textures\\Walls\\Ice\\Albedo_Ice.png"
				, L"Assets\\Textures\\Walls\\Ice\\Normal.png"
				, L"Assets\\Textures\\Walls\\Ice\\Specular.png"
				, L"Assets\\Meshes\\Walls\\wall.obj"
				, L"Vertex_Shader.hlsl"
				, L"Pixel_Shader.hlsl", Vector3D(135, 25, 25), CULL_MODE::CULL_MODE_BACK);
			m_actorManager->SetScale(m_walls[0]->GetEntityID(), Vector3D(10, 10, 10));
			m_actorManager->SetPosition(m_walls[0]->GetEntityID(), Vector3D(-m_ground_tile_size * m_ground_length +67.5, 0,-m_ground_tile_size * m_ground_width ));
			m_actorManager->SetBoundingBoxOfffset(m_walls[0]->GetEntityID(), Vector3D(-2.5, 15, 0));
			m_walls[0]->SetGravity(false);

	m_walls[1] = new Actor();
	m_actorManager->InstantiateActor(m_walls[1], m_walls[0]);

	m_actorManager->SetPosition(m_walls[1]->GetEntityID(), Vector3D(m_walls[0]->GetPosition().m_x , 0, -m_walls[0]->GetPosition().m_z));
	m_walls[1]->SetGravity(false);
    // distibute the walls along the map edges
	int k=2;
	while (k *62 <= m_ground_tile_size * 2 * m_ground_length)
	{
	m_walls.resize(k+2);

	m_walls[k] = new Actor();
	m_actorManager->InstantiateActor(m_walls[k], m_walls[0]);

	m_actorManager->SetPosition(m_walls[k]->GetEntityID(), Vector3D(m_walls[0]->GetPosition().m_x + float(k) * 62, 0, m_walls[0]->GetPosition().m_z));
	m_walls[k]->SetGravity(false);
	k++;

	m_walls[k] = new Actor();
	m_actorManager->InstantiateActor(m_walls[k], m_walls[0]);

	m_actorManager->SetPosition(m_walls[k]->GetEntityID(), Vector3D(m_walls[k-1]->GetPosition().m_x, 0.0f, -m_walls[0]->GetPosition().m_z));

	m_walls[k]->SetGravity(false);
	k++;

	}
	     //second orientation
	m_walls.resize(k+1);
	m_walls[k] = new Actor();
	m_actorManager->registerObject(m_walls[k], "Wall", L"Assets\\Textures\\Walls\\Ice\\Albedo_Ice.png"
		, L"Assets\\Textures\\Walls\\Ice\\Normal.png"
		, L"Assets\\Textures\\Walls\\Ice\\Specular.png"
		, L"Assets\\Meshes\\Walls\\wall.obj"
		, L"Vertex_Shader.hlsl"
		, L"Pixel_Shader.hlsl", Vector3D(25, 25, 135), CULL_MODE::CULL_MODE_BACK);
	m_actorManager->SetScale(m_walls[k ]->GetEntityID(), Vector3D(10, 10, 10));
	m_actorManager->SetRotation(m_walls[k ]->GetEntityID(), Rotator3D(0, 90, 0));

	m_actorManager->SetPosition(m_walls[k]->GetEntityID(), Vector3D(-m_ground_tile_size * m_ground_length, 0,-m_ground_tile_size * m_ground_width + 65 ));

	m_actorManager->SetBoundingBoxOfffset(m_walls[k]->GetEntityID(), Vector3D(0, 15, 2.5));
	m_walls[k ]->SetGravity(false);

	m_walls[k + 1] = new Actor();
	m_actorManager->InstantiateActor(m_walls[k + 1], m_walls[k]);

	m_actorManager->SetPosition(m_walls[k + 1]->GetEntityID(), Vector3D(-m_walls[k]->GetPosition().m_x, 0, m_walls[k]->GetPosition().m_z));
	m_walls[k + 1]->SetGravity(false);


	int k1 = k + 2;
	while ((k1-k) * 62 <= m_ground_tile_size * 2 * m_ground_width)
	{
		m_walls.resize(k1+2 );

		m_walls[k1] = new Actor();
		m_actorManager->InstantiateActor(m_walls[k1], m_walls[k]);

		m_actorManager->SetPosition(m_walls[k1]->GetEntityID(), Vector3D(m_walls[k]->GetPosition().m_x, 0.0f,m_walls[k]->GetPosition().m_z + float(k1-k) * 62 ));
		m_walls[k1]->SetGravity(false);
		k1++;

		m_walls[k1] = new Actor();
		m_actorManager->InstantiateActor(m_walls[k1], m_walls[k ]);

		m_actorManager->SetPosition(m_walls[k1]->GetEntityID(), Vector3D(-m_walls[k1 - 1]->GetPosition().m_x, 0.0f,m_walls[k1 - 1]->GetPosition().m_z ));

		m_walls[k1]->SetGravity(false);
		k1++;

	}
	//TURRETS---------------------------------------------------------------------------------------------

	m_enemy_turrets.resize(m_turret_number);
	m_turret_sockets.resize(m_turret_number);
	m_turrets_in_existance = m_turret_number;

	for (int i = 0; i < m_turret_number; i++)	// generate random turret positions and create them, create the towers and the shooting socket
	{
		int pos_randz;
		int signz;
		int pos_randx;
		int signx;

		pos_randz = rand() % int(m_ground_width * m_ground_tile_size - 150) + 100;
		signz = rand() % 2 + 0;
		if (signz == 0)pos_randz = -pos_randz;

		pos_randx = rand() % int(m_ground_length * m_ground_tile_size - 150) + 100;
		signx = rand() % 2 + 0;
		if (signx == 0)pos_randx = -pos_randx;

		//The turret constructor has an actor as a parameter that is used as a target 
		m_enemy_turrets[i] = new Turret(m_cht->GetEntityID());

		//create the TURRETS
		m_pawnManager->registerObject(m_enemy_turrets[i], "Turret", L"Assets\\Textures\\Turret\\Lava_albedo.jpg",
			L"Assets\\Textures\\Turret\\Lava_normal.jpg",
			NULL,
			L"Assets\\Meshes\\Turret\\TankCannon.obj",
			L"Vertex_Shader.hlsl",
			L"Pixel_Shader.hlsl",
			Vector3D(11, 11, 11), CULL_MODE::CULL_MODE_BACK, 0
		);
		m_actorManager->SetPosition(m_enemy_turrets[i]->GetEntityID(), Vector3D(float(pos_randx), 75, float(pos_randz)));
		m_actorManager->SetScale(m_enemy_turrets[i]->GetEntityID(), Vector3D(12, 12, 12));
		m_actorManager->AddHealthComponent(m_enemy_turrets[i]->GetEntityID());
		m_enemy_turrets[i]->SetGravity(false);

		m_turret_sockets[i] = new Socket(m_enemy_turrets[i]->GetEntityID(), Vector3D(0, 0, 20), Rotator3D(0, 0, 0));

		m_objectManager->registerObject(m_turret_sockets[i], "turret_socket");

		            //create the TURRET TOWER
		m_actorManager->registerObject(m_enemy_turrets[i]->GetTower(), "tower", L"Assets\\Textures\\Walls\\Lava\\Albedo.png"
			, L"Assets\\Textures\\Walls\\Lava\\Normal.png"
			, L"Assets\\Textures\\Walls\\Lava\\Specular.png"
			, L"Assets\\Meshes\\Turret\\Tower.obj"
			, L"Vertex_Shader.hlsl"
			, L"Pixel_Shader.hlsl", Vector3D(15, 70, 15), CULL_MODE::CULL_MODE_BACK);

		m_actorManager->SetScale(m_enemy_turrets[i]->GetTower()->GetEntityID(), Vector3D(15, 15, 15));
		m_actorManager->SetPosition(m_enemy_turrets[i]->GetTower()->GetEntityID(), Vector3D(float(pos_randx), 0, float(pos_randz)));
		m_actorManager->AddHealthComponent(m_enemy_turrets[i]->GetTower()->GetEntityID());
		m_actorManager->SetBoundingBoxOfffset(m_enemy_turrets[i]->GetTower()->GetEntityID(), Vector3D(0, 35, 0));
		m_enemy_turrets[i]->GetTower()->SetGravity(false);
		m_enemy_turrets[i]->SetMaxHealth(40);
	}

	//Projectile meshes---------------------------------------------------------------------------------------------

	m_actorManager->registerObject(m_cht_proj, "Proj", L"Assets\\Textures\\Snow_texture_1\\Snow_Albedo.jpg",
		L"Assets\\Textures\\Snow_texture_1\\Snow_Normal.jpg",
		L"Assets\\Textures\\Snow_texture_1\\SN_2\\Snow_specular.jpg",
		L"Assets\\Meshes\\sphere.obj",
		L"Vertex_Shader.hlsl",
		L"Pixel_Shader.hlsl",
		Vector3D(1, 1, 1), CULL_MODE::CULL_MODE_BACK);
	m_cht_proj->SetCollision(false);
	m_cht_proj->SetGravity(false);
	m_cht_proj->SetMeshVisibility(false);

	m_actorManager->registerObject(m_turret_proj, "Proj", L"Assets\\Textures\\Turret\\Lava_albedo.jpg",
		L"Assets\\Textures\\Turret\\Lava_normal.jpg",
		NULL,
		L"Assets\\Meshes\\sphere.obj",
		L"Vertex_Shader.hlsl",
		L"Pixel_Shader.hlsl",
		Vector3D(1, 1, 1), CULL_MODE::CULL_MODE_BACK);
	m_turret_proj->SetCollision(false);
	m_turret_proj->SetGravity(false);
	m_turret_proj->SetMeshVisibility(false);




	//Create HUD---------------------------------------------------------------------
	      //Buttons---------------------------------------------------------------------------------------------

	m_widget_manager->RegisterButton(b_restart_button, "Restart Buton", restart, Point(width / 2, height / 2 + 50), Point(200, 100), L"Restart", 40, Vector3D(1, 1, 1),
		Vector3D(0.3f, 0.2f, 1), ShapeType::Filled_Borders, 4, Vector3D(1, 1, 1));
	b_restart_button->SetVisibility(false);

	m_widget_manager->RegisterButton(b_main_menu_button, "Menu Buton", exitToMainMenu, Point(width / 2, height / 2 + 170), Point(200, 100), L"Main Menu", 40, Vector3D(1, 1, 1),
		Vector3D(0.3f, 0.2f, 1), ShapeType::Filled_Borders, 4, Vector3D(1, 1, 1));
	b_main_menu_button->SetVisibility(false);
	       
	    //Special widgets---------------------------------------------------------------------------------------------
	m_widget_manager->RegisterText(m_win_lose_message, "Status message", L" You won! The snow is back!", Point(width / 2, height / 3), Point(width, height / 4), Vector3D(0.7f, 0.7f, 1), 70);
	m_win_lose_message->SetVisibility(false);

	m_widget_manager->RegisterText(m_loading_text, "Loading_Text", L" Loading...", Point(width / 2, height / 2), Point(width / 4, height / 4), Vector3D(0.3f, 0.3f, 1), 50);
	m_loading_text->SetVisibility(false);

	m_widget_manager->RegisterText(m_mission_text_message, "Mission", L" My name is Zlorp. Help me destroy the lava towers in order to restore the snow on my land! ",
		Point(width / 2, height / 3), Point(width, height / 4), Vector3D(1, 1, 1), 40);


	     //Gameplay widgets---------------------------------------------------------------------------------------------
    m_widget_manager->RegisterText(m_turrets_remaining_number, "Turret_Number", L" text", Point(width - width / 8, height / 10), Point(width / 10, height / 10), Vector3D(1, 0.4f, 0.3f), 50);
	m_turrets_remaining_number->UpdateText(m_turrets_in_existance);

	m_widget_manager->RegisterText(m_turrets_remaining, "Turrets", L"towers left", Point(width - width / 10, height / 10), Point(width / 5, height / 10), Vector3D(1, 0.4f, 0.3f), 50);
	
	m_widget_manager->RegisterRect(m_crosshair_up, "Crosshair_UP",
		Point(width / 2, height / 2 - 20), Point(4, 20), WidgetPositionOffset::Center, Vector3D(1, 1, 1),
		ShapeType::Filled_Borders, 2, Vector3D(0, 0, 0));
	m_widget_manager->RegisterRect(m_crosshair_down1, "Crosshair_DOWN1",
		Point(width / 2, height / 2 + 20), Point(20, 4), WidgetPositionOffset::Center, Vector3D(1, 1, 1),
		ShapeType::Filled_Borders, 2, Vector3D(0, 0, 0));
	m_widget_manager->RegisterRect(m_crosshair_down2, "Crosshair_DOWN2",
		Point(width / 2, height / 2 + 30), Point(15, 4), WidgetPositionOffset::Center, Vector3D(1, 1, 1),
		ShapeType::Filled_Borders, 2, Vector3D(0, 0, 0));
	m_widget_manager->RegisterRect(m_crosshair_down3, "Crosshair_DOWN3",
		Point(width / 2, height / 2 + 40), Point(10, 4), WidgetPositionOffset::Center, Vector3D(1, 1, 1),
		ShapeType::Filled_Borders, 2, Vector3D(0, 0, 0));
	m_widget_manager->RegisterRect(m_crosshair_right, "Crosshair_Right",
		Point(width / 2 + 20, height / 2), Point(20, 4), WidgetPositionOffset::Center, Vector3D(1, 1, 1),
		ShapeType::Filled_Borders, 2, Vector3D(0, 0, 0));
	m_widget_manager->RegisterRect(m_crosshair_left, "Crosshair_Left",
		Point(width / 2 - 20, height / 2), Point(20, 4), WidgetPositionOffset::Center, Vector3D(1, 1, 1),
		ShapeType::Filled_Borders, 2, Vector3D(0, 0, 0));
	m_widget_manager->RegisterRect(m_crosshair_center, "Crosshair_Center",
		Point(width / 2, height / 2), Point(4, 4), WidgetPositionOffset::Center, Vector3D(1, 1, 1),
		ShapeType::Filled_Borders, 2, Vector3D(0, 0, 0));

	m_widget_manager->RegisterRect(m_health_bar_inside, "Health_Bar",
		Point(int(width / 2 - width / 8), int(height / 2) + int(height / 2)), Point(int(width / 4), int(height / 25)), WidgetPositionOffset::Right, Vector3D(0.7f, 0.7f, 1));
	m_widget_manager->RegisterRect(m_health_bar_outside, "Health_Bar",
		Point(width / 2 - width / 8, int(height / 2 + height / 2.5)), Point(width / 4, height / 25), WidgetPositionOffset::Right, Vector3D(0, 0, 0), ShapeType::Empty, 3);


	// SOUNDS-------------------------------------------------------------------------------------------
	m_snowball_launch_sound->createSound(L"Assets\\Sounds\\Launch_Snowball.wav");
	m_snowball_hit_sound->createSound(L"Assets\\Sounds\\Hit_Snowball.wav");
	m_get_damage_sound->createSound(L"Assets\\Sounds\\Take_damage.wav");
	m_ambiental_music->createSound(L"Assets\\Sounds\\Snowfall – Scott Buckley (No Copyright Music).wav");
	m_ambiental_music->playSound(0.3f);


	Level::BeginPlay();

}

void GameLevel::Tick(float DeltaTime)
{
	Level::Tick(DeltaTime);// call Tick on parent class 
	
	if (!m_play_state)return;

	if (m_gun)
	for (int i = 0; i < m_gun->GetProjectileIndex(); i++)//run over all the projectiles owned by the gun
	{
		if (m_gun->GetProjectileByIndex(i))
		{
			if (m_gun->GetProjectileByIndex(i)->GetProjectileDead())// delete projectile from actorM and from gun if its lifetime ran out
			{
				m_actorManager->deleteActorFromManager(m_gun->GetProjectileByIndex(i));
				m_gun->deleteProjectileByIndex(i);
				
				break;
			}
		//Get BB component by projectile entiy ID
		 auto& bb1 = m_EntityComponentSystemManager->GetComponent<BoundingBox>(m_gun->GetProjectileByIndex(i)->GetEntityID());

		  for (auto const& entity2 : m_actorManager->m_Entities)//run over all actors in the scene
		  {
			  
			auto& bb2 = m_EntityComponentSystemManager->GetComponent<BoundingBox>(entity2);
		
			if(bb2.collision_enabled && (sqaredDistance(bb1.center, bb2.center)<1600 || entity2 == m_ground_tiles[0]->GetEntityID()))//Check collision only if actors are in a range of 40 units(40^2=1600)
				//if projectile actor ID is different than owner id or projectile ID
				//using sqared distance to avoid using sqrt and just compare it with the sqared range
			if (entity2!= m_cht->GetEntityID() && m_gun->GetProjectileByIndex(i)->GetEntityID() != entity2 && bb2.sizeX != 0)
			{
				CollisionResult CRes = m_collision_system->SimpleBBCollisionCheck(bb1, bb2);
			
				   if (CRes.collided)//If collision occured
					{
					   if(m_actorManager->GetActor(entity2)->GetHasHealth())//if hit actor has health component
					   { 
					   auto& health = m_EntityComponentSystemManager->GetComponent<HealthComponent>(entity2);
					   health.DealDamange(m_gun->GetProjectileByIndex(i)->GetDamageAmount());
					   m_snowball_hit_sound->playSound(0.3);
                       }
					   //Delete projectile
						m_actorManager->deleteActorFromManager(m_gun->GetProjectileByIndex(i));
						m_gun->deleteProjectileByIndex(i);
						break;
		
			       }
	        }
		  }
        }
	}
	if (m_turrets_in_existance != 0)
	for (int i = 0; i < m_turrets_in_existance; i++)// Run over all turrets
	{
		
		if (m_enemy_turrets[i])
		{
			
			auto& turret_health = m_EntityComponentSystemManager->GetComponent<HealthComponent>(m_enemy_turrets[i]->GetEntityID());
			
			if (m_enemy_turrets[i]->GetTower())//if the turret tower is not nullptr
			{
				auto& health_tower = m_EntityComponentSystemManager->GetComponent<HealthComponent>(m_enemy_turrets[i]->GetTower()->GetEntityID());
				
				if (health_tower.health == 0)turret_health.health = 0;
			}
			else
			{
				turret_health.health = 0;
			}

		
			if (turret_health.health == 0)
			{
				if (!m_enemy_turrets[i]->GetDisabled())// disable the turret, but not delete it (only if it was not disabled before)
				{ 
                m_enemy_turrets[i]->SetMeshVisibility(false);
				m_enemy_turrets[i]->SetCollision(false);
                m_enemy_turrets[i]->SetDisabled(true);

				//When the turret is destroyed, it shoots 10 projectiles in random directions
				  for (int j = 0; j <=10; j++)
				  {
					float pos_x = float((rand() % 10 + 0)) /10;
					float pos_y = float((rand() % 10 + 0)) / 10;
					float pos_z = float((rand() % 10 + 0)) / 10;
					int sign_x;
					int sign_y;
					int sign_z;


					sign_x = rand() % 2 + 0;
					if (sign_x == 0)pos_x = -pos_x;
					sign_y = rand() % 2 + 0;
					if (sign_y == 0)pos_y = -pos_y;
					sign_z = rand() % 2 + 0;
					if (sign_z == 0)pos_z = -pos_z;

				    TurretShoot(i,Vector3D(pos_x, pos_y, pos_z));
				  } 
				
				m_actorManager->deleteActorFromManager(m_enemy_turrets[i]->GetTower());//delete the tower
				m_enemy_turrets[i]->deleteTower();


                
                m_turrets_remaining_number->UpdateText(m_turrets_in_existance-1);

			
                }
			    if(m_enemy_turrets[i]->GetProjInExistance()==0) // if it is disabled and the number of projectiles owned is 0
					                                                 //(destroy all the projectile before destroying the turret tha owns them) 
				{ 
					//decrement
                    m_turrets_in_existance--;
					//delete socket
					m_objectManager->deleteObjectFromManager(m_turret_sockets[i]);
					delete m_turret_sockets[i];
					m_turret_sockets[i] = m_turret_sockets[int(double(m_turrets_in_existance))];
					m_turret_sockets[int(double(m_turrets_in_existance))] = nullptr;

					//delete turret
					m_pawnManager->deletePawnFromManager(m_enemy_turrets[i]);
					delete m_enemy_turrets[i];
					m_enemy_turrets[i] = m_enemy_turrets[int(double(m_turrets_in_existance))];
					m_enemy_turrets[int(double(m_turrets_in_existance))] = nullptr;


				break;

                }

			}
			
			//the turret shoots if it is not disabled and the target is in range
			if (!m_enemy_turrets[i]->GetDisabled() && m_enemy_turrets[i]->GetCanShoot() && m_enemy_turrets[i]->GetIsInRange())
			{
				TurretShoot(i);
			}
			
			for (int j = 0; j < m_enemy_turrets[i]->GetProjectileIndex(); j++)//run over all projectile owned by a turret
			{
		
				if (m_enemy_turrets[i]->GetProjectileByIndex(j))
				{
					if (m_enemy_turrets[i]->GetProjectileByIndex(j)->GetProjectileDead())//delete projectile if liftime ended
					{
                       
						m_actorManager->deleteActorFromManager(m_enemy_turrets[i]->GetProjectileByIndex(j));
						m_enemy_turrets[i]->deleteProjectileByIndex(j);
					
						break;
					}
	
					auto& bb1 = m_EntityComponentSystemManager->GetComponent<BoundingBox>(m_enemy_turrets[i]->GetProjectileByIndex(j)->GetEntityID());
					for (auto const& entity2 : m_actorManager->m_Entities)
					{
						

						auto& bb2 = m_EntityComponentSystemManager->GetComponent<BoundingBox>(entity2);

						if (bb2.collision_enabled &&(sqaredDistance(bb1.center, bb2.center) < 1600 || entity2 == m_ground_tiles[0]->GetEntityID()))//Check collision only if actors are in a range of 40 units
				        //if projectile actor ID is different than owner id or projectile ID
				        //using sqared distance to avoid using sqrt and just compare it with the sqared range
						if (m_enemy_turrets[i]->GetEntityID()!=entity2 && m_enemy_turrets[i]->GetProjectileByIndex(j)->GetEntityID() != entity2 && bb2.sizeX != 0)
						//if entity2 ID is different from the owner turret and the projectile
						{
							CollisionResult CRes = m_collision_system->SimpleBBCollisionCheck(bb1, bb2);
							



							if (CRes.collided)
							{
						
								if (m_actorManager->GetActor(entity2)->GetHasHealth())
								{
									//deal damage
									auto& health = m_EntityComponentSystemManager->GetComponent<HealthComponent>(entity2);
									health.DealDamange(m_enemy_turrets[i]->GetProjectileByIndex(j)->GetDamageAmount());
									if (m_cht->GetEntityID() == entity2)m_get_damage_sound->playSound(2);

								}
								//destroy projectile
								m_actorManager->deleteActorFromManager(m_enemy_turrets[i]->GetProjectileByIndex(j));
								m_enemy_turrets[i]->deleteProjectileByIndex(j);
								break;

							}
						}
					}
					

				}
			}
		

		}
		

	}


	auto& health = m_EntityComponentSystemManager->GetComponent<HealthComponent>(m_cht->GetEntityID());
	auto& meshComp = m_EntityComponentSystemManager->GetComponent<MeshComponent>(m_cht->GetEntityID());
	auto& gun_meshComp = m_EntityComponentSystemManager->GetComponent<MeshComponent>(m_gun->GetEntityID());
	//Check the state of the game
	if (health.health == 0 && !b_game_lost && !b_game_won)
	{
		meshComp.visible = false;
		gun_meshComp.visible = false;
		m_play_state = false;
		b_game_lost = true;
		b_game_won = false;
		m_cht->SetReceiveInput(false);
		
		b_restart_button->SetVisibility(true);
		b_main_menu_button->SetVisibility(true);
		SetHUDVisibility(false);
        InputSystem::GetInputSystem()->showCursor(true);
	}
	if (m_turrets_in_existance == 0 && !b_game_lost && !b_game_won)
	{
		m_play_state = false;
		b_game_won = true;
		b_game_lost = false;
        m_cht->SetReceiveInput(false);
		
		b_restart_button->SetVisibility(true);
		b_main_menu_button->SetVisibility(true);
		SetHUDVisibility(false);
        InputSystem::GetInputSystem()->showCursor(true);
	}
	  
}



void GameLevel::updateActors(float DeltaTime)
{
	// Set start message to invisible after the character leaves the area
	auto& transform = m_EntityComponentSystemManager->GetComponent<TransformComponent>(m_cht->GetEntityID());
	if (transform.position.m_x > 70 || transform.position.m_x < -70 || transform.position.m_z>70 || transform.position.m_z < -70)m_mission_text_message->SetVisibility(false);


	// Calculate the texture alpha for the ground texture
	
	for (auto const& tiles : m_ground_tiles)//run over all the tiles
	{
	auto& ground_meshComp = m_EntityComponentSystemManager->GetComponent<MeshComponent>(tiles->GetEntityID());

		if (!b_game_lost)
			ground_meshComp.texture_alpha = lerp(ground_meshComp.texture_alpha, 1 - float(m_turrets_in_existance) / float(m_turret_number), DeltaTime);
		else
		{
			ground_meshComp.texture_alpha = lerp(ground_meshComp.texture_alpha, 0, DeltaTime);
		}
	}

	Level::updateActors(DeltaTime);
	
}

void GameLevel::updateWidgets(float DeltaTime)
{
	Level::updateWidgets(DeltaTime);

	//Calculate window sizes
	int width = m_window_rect.right - m_window_rect.left;
	int height = m_window_rect.bottom - m_window_rect.top;
	

	//UPDATE WIDGETS
	      //Buttons
	b_restart_button->UpdatePosition(Point(width / 2, height / 2 + 50 ));
    b_main_menu_button->UpdatePosition(Point(width / 2, height / 2 + 170));
	      //Text widgets
    m_win_lose_message->SetScale(Point(width , height / 4));
	m_win_lose_message->SetPosition(Point(width / 2, height / 3));

    m_mission_text_message->SetScale(Point(width, height / 4));
    m_mission_text_message->SetPosition(Point(width / 2, height / 3));

	m_loading_text->SetPosition(Point(width / 2, height / 2));

    m_turrets_remaining_number->SetPosition(Point(int(width - width / 6), int(height / 10)));
    m_turrets_remaining->SetPosition(Point(int(width - width / 7), int(height / 10)));

    m_turrets_remaining_number->SetScale(Point(int(width / 10), int(height / 10)));
    m_turrets_remaining->SetScale(Point(int(width / 15), int(height / 10)));
	//IF game state changes, enable the according widget 
	if (b_game_won)
	{
		m_win_lose_message->SetVisibility(true);
		SetHUDVisibility(false);
	}
	else if (b_game_lost)
	{
		m_win_lose_message->SetVisibility(true);
		m_win_lose_message->UpdateColor(Vector3D(1,0.4f,0.3f));
		m_win_lose_message->UpdateText(L"You lost!The snow is now gone!");
		SetHUDVisibility(false);
	}
	
    if (!m_play_state)return;

	       //Sqare Widgets
    m_crosshair_up->SetPosition(Point(width / 2, height / 2 - 20));
	m_crosshair_down1->SetPosition(Point(width / 2, height / 2 + 20));
	m_crosshair_down2->SetPosition(Point(width / 2, height / 2 + 30));
	m_crosshair_down3->SetPosition(Point(width / 2, height / 2 + 40));
	m_crosshair_right->SetPosition(Point(width / 2+ 20, height / 2 ));
	m_crosshair_left->SetPosition(Point(width / 2 - 20, height / 2 ));
	m_crosshair_center->SetPosition(Point(width / 2 , height / 2));

    m_health_bar_inside->SetPosition(Point(int(width / 2 - width / 8), int(height / 2 + height / 2.5)));
	m_health_bar_outside->SetPosition(Point(int(width / 2 - width / 8), int(height / 2 + height / 2.5)));

    auto& health = m_EntityComponentSystemManager->GetComponent<HealthComponent>(m_cht->GetEntityID());
    m_health_bar_value = lerp(m_health_bar_value,(health.health / health.max_health) * width / 4, DeltaTime*2);
    m_health_bar_inside->SetScale(Point(int(m_health_bar_value), int(height / 25)));

    m_health_bar_outside->SetScale(Point(int(width / 4), int(height / 25)));

}

void GameLevel::CharacterShoot()
{
	// Cast a ray from the screen midle to the point that is 500 units further in the camera looking direction 
	RayCastResult RRes;
	RRes = m_gun->RayCast(m_world_camera.getTranslation(), m_world_camera.getTranslation() + m_world_camera.getZDirection() * 500);
    Vector3D shoot_direction;

	if (RRes.collided)
	{ // if the ray collided, the projectile is launched in the direction created between  the gun and the hit point of the ray   
		shoot_direction = RRes.hit_point - m_gun_socket->GetPosition();
    }
	else 
	{
		//if the ray did not collide with anything, the launched is released in the camera direction
		shoot_direction =  m_world_camera.getTranslation() + m_world_camera.getZDirection()*500 - m_gun_socket->GetPosition();
	}
	//Create the projectile, with the "Shoot" function that creates a new instance of a projectile
	m_actorManager->InstantiateActor(m_gun->Shoot(Vector3D::normalize(shoot_direction)),m_cht_proj);
	m_gun->GetProjectileByIndex(m_gun->GetProjectileIndex() - 1)->SetCollision(true);
	m_gun->GetProjectileByIndex(m_gun->GetProjectileIndex() - 1)->SetGravity(true);
	m_gun->GetProjectileByIndex(m_gun->GetProjectileIndex() - 1)->SetMeshVisibility(true);
	//place the projectile at the gun socket
	m_actorManager->SetPosition(m_gun->GetProjectileByIndex(m_gun->GetProjectileIndex() - 1)->GetEntityID(), m_gun_socket->GetPosition());
	m_actorManager->SetScale(m_gun->GetProjectileByIndex(m_gun->GetProjectileIndex() - 1)->GetEntityID(), Vector3D(0.7f, 0.7f, 0.7f));
}

void GameLevel::TurretShoot(int turret_index, Vector3D shoot_direction)
{
	// more or less similar to "CharacterShoot" function
	if (shoot_direction.m_x == 0 && shoot_direction.m_y == 0 && shoot_direction.m_z == 0)//if funtion has shoot_direction parameter default
	{
		m_actorManager->InstantiateActor(m_enemy_turrets[turret_index]->Shoot(),m_turret_proj);
		m_enemy_turrets[turret_index]->GetProjectileByIndex(m_enemy_turrets[turret_index]->GetProjectileIndex() - 1)->SetCollision(true);
		m_enemy_turrets[turret_index]->GetProjectileByIndex(m_enemy_turrets[turret_index]->GetProjectileIndex() - 1)->SetGravity(true);
		m_enemy_turrets[turret_index]->GetProjectileByIndex(m_enemy_turrets[turret_index]->GetProjectileIndex() - 1)->SetMeshVisibility(true);
	    m_actorManager->SetPosition(m_enemy_turrets[turret_index]->GetProjectileByIndex(m_enemy_turrets[turret_index]->GetProjectileIndex() - 1)->GetEntityID(), m_turret_sockets[turret_index]->GetPosition());



	}
	else//if funtion has shoot_direction parameter DIFFERENT than default
	{
		m_actorManager->InstantiateActor(m_enemy_turrets[turret_index]->Shoot(shoot_direction), m_turret_proj);
		m_enemy_turrets[turret_index]->GetProjectileByIndex(m_enemy_turrets[turret_index]->GetProjectileIndex() - 1)->SetCollision(true);
		m_enemy_turrets[turret_index]->GetProjectileByIndex(m_enemy_turrets[turret_index]->GetProjectileIndex() - 1)->SetGravity(true);
		m_enemy_turrets[turret_index]->GetProjectileByIndex(m_enemy_turrets[turret_index]->GetProjectileIndex() - 1)->SetMeshVisibility(true);
		m_actorManager->SetPosition(m_enemy_turrets[turret_index]->GetProjectileByIndex(m_enemy_turrets[turret_index]->GetProjectileIndex() - 1)->GetEntityID(), m_actorManager->GetPosition(m_enemy_turrets[turret_index]->GetEntityID())+Vector3D(0,10,0)+ shoot_direction*15);
	}
	m_actorManager->SetScale(m_enemy_turrets[turret_index]->GetProjectileByIndex(m_enemy_turrets[turret_index]->GetProjectileIndex() - 1)->GetEntityID(), Vector3D(0.7f, 0.7f, 0.7f));
	
}

void GameLevel::SetHUDVisibility(bool visibility)
{
	 m_crosshair_up->SetVisibility(visibility);
     m_crosshair_down1->SetVisibility(visibility);
	 m_crosshair_down2->SetVisibility(visibility);
	 m_crosshair_down3->SetVisibility(visibility);
	 m_crosshair_right->SetVisibility(visibility);
	 m_crosshair_left->SetVisibility(visibility);
	 m_crosshair_center->SetVisibility(visibility);
	 m_turrets_remaining_number->SetVisibility(visibility);
	 m_turrets_remaining->SetVisibility(visibility);
	 m_health_bar_inside->SetVisibility(visibility);
	 m_health_bar_outside->SetVisibility(visibility);
}


void GameLevel::onKeyDown(int key)
{
	Level::onKeyDown(key);
	m_cht->onKeyDown(key);
}

void GameLevel::onKeyUp(int key)
{
	Level::onKeyUp(key);

	//Open/Close menu
	if (key == VK_ESCAPE)
	{
		if (m_play_state)
		{
			m_play_state = false;
			m_cht->SetReceiveInput(m_play_state);
			b_restart_button->SetVisibility(true);
			b_main_menu_button->SetVisibility(true);
			SetHUDVisibility(false);
     InputSystem::GetInputSystem()->showCursor(true);
		}
		else if (!m_play_state && !b_game_lost && !b_game_won)
		{
			m_play_state = true;
			m_cht->SetReceiveInput(m_play_state);
			b_restart_button->SetVisibility(false);
			b_main_menu_button->SetVisibility(false);
			SetHUDVisibility(true);
            InputSystem::GetInputSystem()->showCursor(false);
		}
	
	}

	m_cht->onKeyUp(key);

}

void GameLevel::onMouseMove(const Point& mouse_pos)
{
	Level::onMouseMove(mouse_pos);
	if (!m_play_state) return;

	int width = m_window_rect.right - m_window_rect.left;
	int height = m_window_rect.bottom - m_window_rect.top;
	InputSystem::GetInputSystem()->setCursorPosition(Point(m_window_rect.left + (int)(width / 2.0f), m_window_rect.top + (int)(height / 2.0f)));

	m_cht->onMouseMove(m_window_rect, mouse_pos);

}

void GameLevel::onLeftMouseDown(const Point& mouse_pos)
{
	Level::onLeftMouseDown(mouse_pos);
	if (!m_play_state) return;
	

	if(m_gun->GetCanShoot())
	{ 
		CharacterShoot();

		m_snowball_launch_sound->playSound(0.3);
    }


}



void GameLevel::onLeftMouseUp(const Point& mouse_pos)
{
	Level::onLeftMouseUp(mouse_pos);
	if (!m_play_state) return;
}

void GameLevel::onRightMouseDown(const Point& mouse_pos)
{
	Level::onRightMouseDown(mouse_pos);	



	if (!m_play_state) return;
}

void GameLevel::onRightMouseUp(const Point& mouse_pos)
{
	Level::onRightMouseUp(mouse_pos);
	if (!m_play_state) return;
}

void GameLevel::exitToMainMenu()
{
	m_loading_text->SetVisibility(true);
	InputSystem::GetInputSystem()->showCursor(true);
	m_active_level = Levels::EMainMenu;
	b_change_level = true;
	m_ambiental_music->stopSound();

}

void GameLevel::restart()
{
	m_loading_text->SetVisibility(true);
	InputSystem::GetInputSystem()->showCursor(false);
	b_restart = true;
	m_ambiental_music->stopSound();

}

bool GameLevel::GetRestart()
{
	return b_restart;
}

GameLevel::~GameLevel()
{

}