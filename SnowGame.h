#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputListener.h"
#include "Matrix4x4.h"
#include "DebuggingTools.h"
#include "HUD.h"
#include "Level.h"
#include "GameLevel.h"
#include "MainMenuLevel.h"

class SnowGame : public Window, public InputListener
{
public:
	SnowGame();
	~SnowGame();


	// Inherited via Window
	virtual void OnCreate() override;

	virtual void OnUpdate() override;

	virtual void OnDestroy() override;

	virtual void OnFocus() override;

	virtual void OnSize() override;

	virtual void OnKillFocus() override;

	// Inherited via InputListener
	virtual void onKeyDown(int key) override;

	virtual void onKeyUp(int key) override;

	virtual void onMouseMove(const Point& mouse_pos) override;

	virtual void onLeftMouseDown(const Point& mouse_pos)  override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;

	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;


	//Image Rendering
	void render();
	void update();
	
private:
	

	SwapChainPtr m_swap_chain = nullptr;

	//3D SCENE
	TexturePtr m_render_target = nullptr;
	TexturePtr m_depth_stencil = nullptr;

	//HUD
	TexturePtr m_HUD_render_target = nullptr;
	TexturePtr m_HUD_depth_stencil = nullptr;
	IDXGISurface* m_HUD_rt_surface = nullptr;

	//QUAD
	MeshPtr m_quad_mesh = nullptr;
	MaterialPtr m_screen_quad_mat = nullptr;
    std::vector<MaterialPtr> m_list_materials;

	//TIME
	float m_old_time = 0;
	float m_current_time = 0;
	float m_delta_time = 0;
	float m_high_delta_limit = 0.0167f;
	float m_low_delta_limit = 0.1f;


	//INPUT
	bool m_rm_pressed = false;
	bool m_lm_pressed = false;


	//LEVELS
	Level* m_active_level  = nullptr;
	GameLevel* m_game_level= new GameLevel;
	MainMenuLevel* m_menu_level = new MainMenuLevel;


    void CreateGameLevel();
	void CreateMenuLevel();


};
