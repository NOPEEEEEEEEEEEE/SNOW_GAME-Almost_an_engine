#pragma once
#include "Actor.h"
#include "Window.h"
#include <iostream>


struct ControllerComponent
{
	Vector3D position;
	Rotator3D rotation;
	float m_forward_axis = 0.0f;
	float m_rightward_axis = 0.0f;
	float delta_mouse_x = 0.0f;
	float delta_mouse_y = 0.0f;
	float mouse_sensitivity = 2.0f;
	bool receive_input = true;

};

class Pawn : public Actor
{
public:
	Pawn()
	{

	}
	
	~Pawn()
	{
		
	}
	
	virtual void Tick(float DeltaTime) override
	{
		Actor::Tick(DeltaTime);
	}
	virtual void BeginPlay() override
	{
		Actor::BeginPlay();

		

	}
	void onKeyDown(int key)
	{
		auto& controller = m_EntityComponentSystemManager->GetComponent<ControllerComponent>(m_ID);
		if (!controller.receive_input)return;
		if (key == 'W')
		{
			SetForwardAxisValue(1.0f);
		}
		else if (key == 'S')
		{
		
			SetForwardAxisValue(-1.0f);
		}

		else if (key == 'A')
		{
			SetRightwardAxisValue(1.0f);
		}
		else if (key == 'D')
		{
			SetRightwardAxisValue(-1.0f);

		}
		else if (key == VK_SPACE)
		{
			if (!m_jump_triggered && m_is_grounded)
			{
				auto& physics = m_EntityComponentSystemManager->GetComponent<PhysicsComponent>(m_ID);
				physics.velocity = Vector3D(0, 30, 0);
				m_is_grounded = false;
				m_jump_triggered = true;
			}

		}

	}
	void onMouseMove(RECT window_rect,const Point& mouse_pos)
	{
		auto& controller = m_EntityComponentSystemManager->GetComponent<ControllerComponent>(m_ID);

		if (!controller.receive_input)return;

		int width = window_rect.right - window_rect.left;
		int height = window_rect.bottom - window_rect.top;
		if (m_mouse_pos.m_x != mouse_pos.m_x)
			controller.delta_mouse_x = (float)(mouse_pos.m_x - (int)(window_rect.left + (width / 2.0f)));
		else controller.delta_mouse_x = 0;

		if(m_mouse_pos.m_y != mouse_pos.m_y)
		controller.delta_mouse_y = (float)(mouse_pos.m_y - (int)(window_rect.top + (height / 2.0f)));
		else controller.delta_mouse_y = 0;

	
		m_mouse_pos.m_x = mouse_pos.m_x;
		m_mouse_pos.m_y = mouse_pos.m_y;
	}
	void onKeyUp(int key)
	{
		auto& controller = m_EntityComponentSystemManager->GetComponent<ControllerComponent>(m_ID);
		if (!controller.receive_input)return;
		SetForwardAxisValue(0.0f);
		SetRightwardAxisValue(0.0f);

		 if (key == VK_SPACE)
		{
			 m_jump_triggered = false;
		}
		
			
	}
	void SetReceiveInput(bool play_state)
	{
		auto& controller = m_EntityComponentSystemManager->GetComponent<ControllerComponent>(m_ID);
		controller.receive_input = play_state;

	}

	void SetForwardAxisValue(float AxisValue)
	{
		auto& controller = m_EntityComponentSystemManager->GetComponent<ControllerComponent>(m_ID);
		controller.m_forward_axis=AxisValue;
	}
	void SetRightwardAxisValue(float AxisValue)
	{
		auto& controller = m_EntityComponentSystemManager->GetComponent<ControllerComponent>(m_ID);
		controller.m_rightward_axis = AxisValue;
	}
	bool GetCanFly()
	{
		return m_can_fly;
	}
	void SetCanFly(bool can_fly)
	{
		m_can_fly = can_fly;
	}

protected:

	bool m_can_fly = false;
	bool m_jump_triggered = false;
	Point m_mouse_pos{};

private:




};