#include "SnowGame.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Vector2D.h"
#include "Rotator3D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "Mesh.h"
#include "MathUtilities.h"
#include "DebuggingTools.h"
#include <iostream>

struct vertex
{
	Vector3D position;
	Vector2D texcoord;
	
};

SnowGame::SnowGame()
{

}

void SnowGame::OnCreate()
{

	Window::OnCreate();


	InputSystem::GetInputSystem()->addListener(this);////ADD WINDOW AS LISTENER TO THE INPUT SYSTEM

	RECT rect = this->GetClientWindowRect();
	m_swap_chain = GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->createSwapChain(this->m_hwnd, rect.right - rect.left, rect.bottom - rect.top);


	HUD::GetHUDSystem()->setScreenRect(rect);



	//--------------------------------------SETUP QUAD FOR FRAME BUFFER---------------------------------------------------------------------

				//SETUP THE MATERIAL FOR THE SCREEN QUAD
	m_screen_quad_mat = GraphicsEngine::GetGraphicsEngine()->createMaterial(L"PostProcessVS.hlsl", L"PostProcessDefaultPS.hlsl");
	m_screen_quad_mat->setCullMode(CULL_MODE::CULL_MODE_BACK);
	//-------------


	VertexMesh quad_vertex_list[] =
	{
		VertexMesh(Vector3D(-1,-1,0),Vector2D(0,1),Vector3D(),Vector3D(),Vector3D()),
		VertexMesh(Vector3D(-1,1,0),Vector2D(0,0),Vector3D(),Vector3D(),Vector3D()),
		VertexMesh(Vector3D(1,1,0),Vector2D(1,0),Vector3D(),Vector3D(),Vector3D()),
		VertexMesh(Vector3D(1,-1,0),Vector2D(1,1),Vector3D(),Vector3D(),Vector3D()),

	};

	unsigned int quad_index_list[] =
	{
		0,1,2,
		2,3,0

	};

	MaterialSlot quad_mat_slots[] =
	{
		{0,6,0}
	};

	m_quad_mesh = GraphicsEngine::GetGraphicsEngine()->getMeshManager()->createMesh(quad_vertex_list, 4, quad_index_list, 6, quad_mat_slots, 1);


	m_list_materials.reserve(32);
	//------------------------------------------------------------------------------------------------------------------------------

	/////////CREATE SCENE RENDER TARGET/DEPTH STENCIL ///////////////////////////////////////////////////////
	m_render_target = GraphicsEngine::GetGraphicsEngine()->getTextureManager()
		->createTexture(Rect(rect.right - rect.left, rect.bottom - rect.top), Texture::Type::RenderTarget);
	m_depth_stencil = GraphicsEngine::GetGraphicsEngine()->getTextureManager()
		->createTexture(Rect(rect.right - rect.left, rect.bottom - rect.top), Texture::Type::DepthStencil);

	/////////CREATE HUD RENDER TARGET/DEPTH STENCIL ///////////////////////////////////////////////////////

	m_HUD_render_target = GraphicsEngine::GetGraphicsEngine()->getTextureManager()
		->createTexture(Rect(rect.right - rect.left, rect.bottom - rect.top), Texture::Type::RenderTarget);
	m_HUD_depth_stencil = GraphicsEngine::GetGraphicsEngine()->getTextureManager()
		->createTexture(Rect(rect.right - rect.left, rect.bottom - rect.top), Texture::Type::DepthStencil);
	m_HUD_render_target->m_texture->QueryInterface(&m_HUD_rt_surface);
	HUD::GetHUDSystem()->BindSurfaceToHUDRT(m_HUD_rt_surface);

	/////////////ADD THE RENDER TARGETS AS TEXTURES TO THE SCREEN QUAD
	m_screen_quad_mat->addTexture(m_render_target);
	m_screen_quad_mat->addTexture(m_HUD_render_target);


	CreateMenuLevel();



}
void SnowGame::OnUpdate()
{

	if (m_menu_level == m_active_level)
		if (m_menu_level->GetExit())
		{
			OnDestroy();
		}

	Window::OnUpdate();
	InputSystem::GetInputSystem()->update();

	m_active_level->SetWindowRect(this->GetClientWindowRect());

	this->update();
	this->render();


}
void SnowGame::update()
{

	if (m_game_level->GetRestart())
	{

		delete m_game_level;
		m_game_level = nullptr;
		CreateGameLevel();
	}

	if (m_active_level->GetActiveLevel() == Levels::EGameLevel && m_active_level->GetChangeLevel())
	{

		CreateGameLevel();
		delete m_menu_level;
		m_menu_level = nullptr;
	}
	else if (m_active_level->GetActiveLevel() == Levels::EMainMenu && m_active_level->GetChangeLevel())
	{
		CreateMenuLevel();
		delete m_game_level;
		m_game_level = nullptr;

	}
	m_active_level->Tick(m_delta_time);


}


void SnowGame::render()
{
	
	//SET VIEWPORT
	Rect viewport_size = m_render_target->getSize();
	GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->getImmediateDeviceContext()
		->setViewportSize(viewport_size.width, viewport_size.height);




    //SET SCENE RENDER TARGET----------------------------------------------------------
	GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->getImmediateDeviceContext()
		->clearRenderTargetColor(this->m_render_target, 0.5, 0.3f, 0.4f, 1);
	GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->getImmediateDeviceContext()
		->clearDepthStencil(this->m_depth_stencil);
	GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->getImmediateDeviceContext()
		->setRenderTarget(this->m_render_target, this->m_depth_stencil);


	//UPDATE ACTOR DATA ON ACTIVE LEVEL
	m_active_level->updateActors(m_delta_time);

	//SET HUD RENDER TARGET-----------------------------------------------------------
	GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->getImmediateDeviceContext()
		->clearRenderTargetColor(this->m_HUD_render_target, 0, 0, 0, 0);
	GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->getImmediateDeviceContext()
		->clearDepthStencil(this->m_HUD_depth_stencil);
    GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->getImmediateDeviceContext()
		->setRenderTarget(this->m_HUD_render_target, m_HUD_depth_stencil);

    //UPDATE HUD DATA ON ACTIVE LEVEL 
	m_active_level->updateWidgets(m_delta_time);

	//CLEAR THE RENDER TARGET 
	GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0, 0, 1);

	//SET VIEWPORT OF RENDER TARGET 
	RECT rc = this->GetClientWindowRect();
	GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

    //DRAW SCREEN QUAD 
	m_list_materials.clear();
	m_list_materials.push_back(m_screen_quad_mat);
	GraphicsEngine::GetGraphicsEngine()->drawMesh(m_quad_mesh, m_list_materials);

	//PRESENT IMAGE
	m_swap_chain->present(true);
	
	
	//CALCULATE DELTA TIME
    m_current_time = float(GetTickCount64());

	m_delta_time = (m_current_time - m_old_time)/ 1000.0f;

	if (m_delta_time > m_low_delta_limit)m_delta_time = m_low_delta_limit;
	else if (m_delta_time < m_high_delta_limit)m_delta_time = m_high_delta_limit;

	m_old_time = m_current_time;
}



void SnowGame::CreateGameLevel()
{
	m_game_level = new GameLevel;
	m_active_level = m_game_level;
	m_active_level->SetWindowRect(this->GetClientWindowRect());
	m_active_level->BeginPlay();
	m_active_level->SetActiveLevel(Levels::EGameLevel);
}

void SnowGame::CreateMenuLevel()
{
	m_menu_level = new MainMenuLevel;
	m_active_level = m_menu_level;
	m_active_level->SetWindowRect(this->GetClientWindowRect());
	m_active_level->BeginPlay();
	m_active_level->SetActiveLevel(Levels::EMainMenu);
	
}

void SnowGame::OnKillFocus()
{
	//REMOVE THE WINDOW FROM THE LISTENER LIST ON INPUT SYSTEM -- ON KILL FOCUS EVENT
	InputSystem::GetInputSystem()->removeListener(this);

}

void SnowGame::onKeyDown(int key)
{
	//CALL KEY DOWN FUNCTION ON ACTIVE LEVEL
	m_active_level->onKeyDown(key);

}

void SnowGame::onKeyUp(int key)
{
	m_active_level->onKeyUp(key);
	
}

void SnowGame::onMouseMove(const Point& mouse_pos)
{
	//SET MOUSE POSITION ON THE ACTIVE LEVEL
	m_active_level->onMouseMove(mouse_pos);
	
}

void SnowGame::onLeftMouseDown(const Point& mouse_pos)
{
	
	if(!m_lm_pressed)
	m_active_level->onLeftMouseDown(mouse_pos);

    m_lm_pressed = true;
}

void SnowGame::onLeftMouseUp(const Point& mouse_pos)
{

	m_active_level->onLeftMouseUp(mouse_pos);

	m_lm_pressed = false;

}

void SnowGame::onRightMouseDown(const Point& mouse_pos)
{
	if (!m_rm_pressed)
	m_active_level->onRightMouseDown(mouse_pos);
	m_rm_pressed = true;

}

void SnowGame::onRightMouseUp(const Point& mouse_pos)
{
	m_active_level->onRightMouseUp(mouse_pos);
	m_rm_pressed = false;

}

void SnowGame::OnFocus()
{
	//ADD THE WINDOW TO THE LISTENER LIST ON INPUT SYSTEM -- ON FOCUS EVENT

	InputSystem::GetInputSystem()->addListener(this);
}

void SnowGame::OnSize()
{

	//RESIZE SWAP CHAIN----------------------------------------------------
	RECT rect = this->GetClientWindowRect();
	m_swap_chain->resize(rect.right - rect.left, rect.bottom - rect.top);
	m_active_level->SetWindowRect(this->GetClientWindowRect());



	/*RECT size_screen = this->GetScreenSize();
    m_swap_chain->setFullScreen(true, size_screen.right, size_screen.bottom);
	if(size_screen.right== rect.right && size_screen.bottom == rect.bottom)
	{ 
	
    }
	else m_swap_chain->setFullScreen(false, rect.right, rect.bottom);*/


	//SET-UP THE RENDER TRAGETS/DEPTH STENCILS WITH THE NEW SIZES---------------------------------------
	     //3D SCENE
	m_render_target = GraphicsEngine::GetGraphicsEngine()->getTextureManager()
		->createTexture(Rect(rect.right - rect.left, rect.bottom - rect.top), Texture::Type::RenderTarget);
	m_depth_stencil = GraphicsEngine::GetGraphicsEngine()->getTextureManager()
		->createTexture(Rect(rect.right - rect.left, rect.bottom - rect.top), Texture::Type::DepthStencil);
	     //HUD
	m_HUD_render_target = GraphicsEngine::GetGraphicsEngine()->getTextureManager()
		->createTexture(Rect(rect.right - rect.left, rect.bottom - rect.top), Texture::Type::RenderTarget);
	m_HUD_depth_stencil = GraphicsEngine::GetGraphicsEngine()->getTextureManager()
		->createTexture(Rect(rect.right - rect.left, rect.bottom - rect.top), Texture::Type::DepthStencil);
	m_HUD_render_target->m_texture->QueryInterface(&m_HUD_rt_surface);
	HUD::GetHUDSystem()->BindSurfaceToHUDRT(m_HUD_rt_surface);
	HUD::GetHUDSystem()->setScreenRect(rect);


	//REMOVE THE RENDER TARGET TEXTURES FROM THE QUAD MATERIAL
	m_screen_quad_mat->removeTexture(0);
	m_screen_quad_mat->removeTexture(0);
	//ADD THE NEW, RESIZED RENDER TARGET TEXTURES TO THE QUAD MATERIAL
	m_screen_quad_mat->addTexture(m_render_target);
	m_screen_quad_mat->addTexture(m_HUD_render_target);


	this->update();
	this->render();
	

}
void SnowGame::OnDestroy()
{
	Window::OnDestroy();
	m_swap_chain->setFullScreen(false, 1, 1);
	
}

SnowGame::~SnowGame()
{
	
}


