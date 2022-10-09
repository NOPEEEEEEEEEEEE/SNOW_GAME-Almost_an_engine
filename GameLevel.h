#pragma once
#include "Level.h"
#include "Sound.h"
#include "Socket.h"
#include "Projectile.h"
#include "SnowLauncher.h"
#include "Turret.h"

enum Difficulty
{
	Easy,
	Medium,
	Hard

};

class GameLevel: public Level
{
public:
	GameLevel();
	~GameLevel();

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

	bool GetRestart();

protected:
	

private:
	//Static functions
	static void exitToMainMenu();
	static void restart();
	static bool b_restart ;

	void CharacterShoot();
	void TurretShoot(int turret_index, Vector3D shoot_direction = Vector3D(0,0,0));

	
	//Objects
	//Actor* m_ground = new Actor;
	Actor* m_sky_box = new Actor;
	Actor* m_cht_proj = new Actor;
	Actor* m_turret_proj = new Actor;

    Character* m_cht = new Character;
	SnowLauncher* m_gun = new SnowLauncher;
    Socket* m_body_socket =nullptr;
	Socket* m_hand_socket = nullptr;
	Socket* m_gun_socket = nullptr;
	      //-------------->Object vectors
	std::vector<Actor*>m_trees{};
	

	std::vector<Actor*>m_walls{};
	

    std::vector<Turret*> m_enemy_turrets{};
	std::vector<Socket*> m_turret_sockets{};
	std::vector<Actor*> m_ground_tiles{};

	            //Object data
	int m_ground_width = 10;
	int m_ground_length = 10;
	float m_ground_tile_size = 100;

	            //Instantiation constant data
	float m_tree_density = 4;//instances per tile
	float m_turret_density = 0.4;//instances per tile
	            //Instantiation  data
      int m_tree_number = 0;
	  int m_turret_number =0;

	            //Instantiation data
	int m_turrets_in_existance= 0;

	
   //HUD---------------------------------------------------------------------
	void SetHUDVisibility(bool visibility);

	static TextWidget* m_loading_text;

	TextWidget* m_turrets_remaining_number = new TextWidget;
    TextWidget* m_turrets_remaining = new TextWidget;
	TextWidget* m_win_lose_message = new TextWidget;
	TextWidget* m_mission_text_message = new TextWidget;
	 

	RectangleWidget* m_crosshair_up = new RectangleWidget;
	RectangleWidget* m_crosshair_down1 = new RectangleWidget;
	RectangleWidget* m_crosshair_down2 = new RectangleWidget;
	RectangleWidget* m_crosshair_down3 = new RectangleWidget;
	RectangleWidget* m_crosshair_right = new RectangleWidget;
	RectangleWidget* m_crosshair_left = new RectangleWidget;
	RectangleWidget* m_crosshair_center = new RectangleWidget;

	RectangleWidget* m_health_bar_inside = new  RectangleWidget;
	RectangleWidget* m_health_bar_outside = new  RectangleWidget;
	float m_health_bar_value = 0;

	Button* b_restart_button = new Button;
	Button* b_main_menu_button = new Button;

	//AUDIO--------------------------------------------------

    static Sound* m_ambiental_music;
	Sound* m_snowball_launch_sound = new Sound;
	Sound* m_snowball_hit_sound = new Sound;
	Sound* m_get_damage_sound = new Sound;

	


	//Game state Bools----------------------------------

	bool b_game_lost = false;
	bool b_game_won = false;


	//Debug Tools
	DebuggingTools* debuggtool = new DebuggingTools;
	
};

