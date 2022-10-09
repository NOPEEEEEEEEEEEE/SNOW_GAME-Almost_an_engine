#include "MainMenuLevel.h"

bool MainMenuLevel::b_exit_game = false;
Sound* MainMenuLevel::m_ambiental_music = new Sound;
TextWidget* MainMenuLevel::m_loading_text = new TextWidget;
MainMenuLevel::MainMenuLevel()
{
	m_this_level = Levels::EMainMenu;
	//m_play_state = false;
}

MainMenuLevel::~MainMenuLevel()
{

}

void MainMenuLevel::Tick(float DeltaTime)
{
	Level::Tick(DeltaTime);
	

}

void MainMenuLevel::BeginPlay()
{
	int width = m_window_rect.right - m_window_rect.left;
	int height = m_window_rect.bottom - m_window_rect.top;

	m_actorManager->registerObject(m_sky, "Sky", L"Assets\\Textures\\HDRI\\Snow_mountains_HDRI.jpg", NULL, NULL
		, L"Assets\\Meshes\\sphere.obj", L"SkyBoxVertexShader.hlsl", L"SkyBoxPixelShader.hlsl"
		, Vector3D(0, 0, 0), CULL_MODE::CULL_MODE_FRONT);
    m_actorManager->SetScale(m_sky->GetEntityID(), Vector3D(4000, 4000, 4000));
	m_sky->SetGravity(false);

	m_actorManager->registerObject(m_ground, "Ground",    L"Assets\\Textures\\Snow_texture_1\\Snow_Albedo.jpg"
		, L"Assets\\Textures\\Snow_texture_1\\Snow_Normal.jpg"
		, L"Assets\\Textures\\Snow_texture_1\\SN_2\\Snow_specular.jpg"
		, L"Assets\\Meshes\\terrain.obj"
		, L"Snow_Ground_VertexShader.hlsl"
		, L"Snow_Ground_PixelShader.hlsl", Vector3D(0, 0, 0), CULL_MODE::CULL_MODE_BACK);

	m_actorManager->SetScale(m_ground->GetEntityID(), Vector3D(1000, 1, 1000));
	m_actorManager->SetPosition(m_ground->GetEntityID(), Vector3D(0, -11, 0));
	m_ground->SetGravity(false);

	m_actorManager->registerObject(m_zlorp, "Zlorp", L"Assets\\Textures\\Character_Texture\\Zlorp_Albedo.png",
		L"Assets\\Textures\\Character_Texture\\Zlorp_Normal.png",
		L"Assets\\Textures\\Character_Texture\\Zlorp_Specular.png",
		L"Assets\\Meshes\\Characters\\Zlorp.obj",
		L"Vertex_Shader.hlsl",
		L"Pixel_Shader.hlsl", Vector3D(0, 0, 0), CULL_MODE::CULL_MODE_BACK);
    m_actorManager->SetScale(m_zlorp->GetEntityID(), Vector3D(10, 10, 10));
	m_actorManager->SetPosition(m_zlorp->GetEntityID(), Vector3D(-1, -10, 13));
	m_actorManager->SetRotation(m_zlorp->GetEntityID(), Rotator3D(0, 150, 0));
	m_ground->SetGravity(false);

	m_actorManager->registerObject(m_gun, "Gun",  L"Assets\\Textures\\Gun_textures\\Gun_Albedo.jpg",
		L"Assets\\Textures\\Gun_textures\\Gun_Normal.jpg",
		L"Assets\\Textures\\Gun_textures\\Gun_Specular.jpg",
		L"Assets\\Meshes\\Weapons\\SnowGun.obj",
		L"Vertex_Shader.hlsl",
		L"Pixel_Shader.hlsl",
		Vector3D(0, 0, 0), CULL_MODE::CULL_MODE_BACK);
	m_actorManager->SetScale(m_gun->GetEntityID(), Vector3D(2.5, 2.5, 2.5));
	m_actorManager->SetPosition(m_gun->GetEntityID(), Vector3D(-0.1f, 4, 10));
	m_actorManager->SetRotation(m_gun->GetEntityID(), Rotator3D(0, 150, 0));

	m_widget_manager->RegisterText(m_snow_text, "Snow", L" Snow", Point(width / 2, height / 6), Point(width, height / 4), Vector3D(1, 1, 1), 200);
	m_widget_manager->RegisterText(m_loading_text, "Loading_Text", L" Loading...", Point(width / 2, height / 2), Point(width/4, height / 4), Vector3D(0.3f, 0.3f, 1), 50);
	m_loading_text->SetVisibility(false);
	
	m_widget_manager->RegisterButton(m_play_button, "Start_Button", MainMenuLevel::Start, Point(300, 300), Point(200, 100), L"Start", 40, Vector3D(1, 1, 1), Vector3D(0.3f, 0.3f, 1), ShapeType::Filled_Borders, 4, Vector3D(0, 0, 0));

	m_widget_manager->RegisterButton(m_exit_button, "Exit_Button", MainMenuLevel::Exit, Point(300, 430), Point(180, 100), L"Exit", 40, Vector3D(1, 1, 1), Vector3D(1, 0.3f, 0.3f), ShapeType::Filled_Borders, 4, Vector3D(0, 0, 0));



	m_ambiental_music->createSound(L"Assets\\Sounds\\Asher Fulero - Night Snow.wav");


	m_ambiental_music->playSound(0.4f);

    Level::BeginPlay();
  
}

void MainMenuLevel::onKeyDown(int key)
{
	Level::onKeyDown(key);

}

void MainMenuLevel::onKeyUp(int key)
{
	Level::onKeyUp(key);


}

void MainMenuLevel::onMouseMove(const Point& mouse_pos)
{
	Level::onMouseMove(mouse_pos);
	
}

void MainMenuLevel::onLeftMouseDown(const Point& mouse_pos)
{
	Level::onLeftMouseDown(mouse_pos);

}

void MainMenuLevel::onLeftMouseUp(const Point& mouse_pos)
{
	Level::onLeftMouseUp(mouse_pos);
}

void MainMenuLevel::onRightMouseDown(const Point& mouse_pos)
{
	Level::onRightMouseDown(mouse_pos);
}

void MainMenuLevel::onRightMouseUp(const Point& mouse_pos)
{
	Level::onRightMouseUp(mouse_pos);
}

void MainMenuLevel::updateActors(float DeltaTime)
{
	Level::updateActors( DeltaTime);
	//m_play_state = false;
	m_play_state = false;
}

void MainMenuLevel::updateWidgets(float DeltaTime)
{
	Level::updateWidgets(DeltaTime);

	int width = m_window_rect.right - m_window_rect.left;
	int height = m_window_rect.bottom - m_window_rect.top;


	m_loading_text->SetPosition(Point(width / 2, height / 2));
	m_snow_text->SetPosition(Point(width / 2, height / 6));

}

bool MainMenuLevel::GetExit()
{
	return b_exit_game;
}

void MainMenuLevel::Start()
{
    InputSystem::GetInputSystem()->showCursor(false);
    m_loading_text->SetVisibility(true);
	m_active_level = Levels::EGameLevel;
	b_change_level = true;
	m_ambiental_music->stopSound();
	
}
void MainMenuLevel::Exit()
{
	
	b_exit_game = true;

	
}
