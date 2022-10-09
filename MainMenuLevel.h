#pragma once
#include "Level.h"
#include "Sound.h"

class MainMenuLevel : public Level
{
public:
	MainMenuLevel();
	~MainMenuLevel();

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	virtual void onKeyDown(int key) override;

	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point& mouse_pos) override;

	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;

	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;

	virtual void updateActors(float DeltaTime) override;
	virtual void updateWidgets(float DeltaTime) override;

	bool GetExit();


private:

	static void Start();
	static void Exit();
	static bool b_exit_game;

	TextWidget* m_snow_text = new TextWidget;
	static TextWidget* m_loading_text;

	Button* m_play_button = new Button;
	Button* m_exit_button = new Button;
	
	Actor* m_zlorp = new Actor;
	Actor* m_ground = new Actor;
	Actor* m_sky = new Actor;
	Actor* m_gun = new Actor;

	DebuggingTools* debuggtool = new DebuggingTools;

	static Sound* m_ambiental_music;

};

