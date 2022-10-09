#pragma once
#include "SceneObjectManager.h"
#include "SceneActorManager.h"
#include "ScenePawnManager.h"
#include "SceneCharacterManager.h"
#include "WidgetManager.h"
#include "Material.h"
#include "DebuggingTools.h"
#include "AudioEngine.h"
#include "Point.h"
#include "EntityComponentSystemManager.h"
#include "InputSystem.h"

#include "CollisionDetectionSystem.h"

enum class Levels
{
	EGameLevel,
	EMainMenu

};


class Level
{
public:
	Level();


	~Level();
	virtual void Tick(float DeltaTime);
	virtual void BeginPlay();

    virtual void onKeyDown(int key);

	virtual void onKeyUp(int key);
	virtual void onMouseMove(const Point& mouse_pos);

	virtual void onLeftMouseDown(const Point& mouse_pos);
	virtual void onLeftMouseUp(const Point& mouse_pos);

	virtual void onRightMouseDown(const Point& mouse_pos);
	virtual void onRightMouseUp(const Point& mouse_pos);


	virtual void updateActors(float DeltaTime);
	virtual void updateWidgets(float DeltaTime);


	
	void SetCameraOwner(Character* camera_owner); 

	void SetActiveLevel(Levels active_level);

	Levels GetActiveLevel();

	//Character* GetCameraOwner(); 


	bool getPlayState();
	void setPlayState(bool state);

	void SetWindowRect(RECT rect);
	bool GetChangeLevel();

protected:
	RECT m_window_rect;

	Levels m_this_level;

	static Levels m_active_level ;
	static bool b_change_level;

	bool m_play_state = false;

	void setUpObjectData();
	


	std::shared_ptr<SceneObjectManager>m_objectManager  ;
	std::shared_ptr<SceneActorManager>m_actorManager;
	std::shared_ptr<ScenePawnManager>m_pawnManager;
	std::shared_ptr<SceneCharacterManager>m_characterManager;

    std::shared_ptr<CollisionDetectionSystem>m_collision_system;

	std::shared_ptr<WidgetManager> m_widget_manager;

	
	Matrix4x4 m_light_rotation_matrix;
	Vector4D m_light_position;

	Character* m_camera_owner = nullptr;

    Matrix4x4 m_world_camera;
	Matrix4x4 m_view_camera;
	Matrix4x4 m_proj_camera;

    EntityComponentSystemManager* m_EntityComponentSystemManager = new EntityComponentSystemManager;
	
   //HUD
	TextWidget* m_frames_per_second = new TextWidget;

private:
	


    void updateCamera();
	void updateLight(); 

	

    std::vector<MaterialPtr> m_list_materials;

	bool b_has_objects = true;
	bool m_mouse_L_pressed = false;
	Point m_cursor_window_position{};

	DebuggingTools* debuggtool = new DebuggingTools;

    

};

