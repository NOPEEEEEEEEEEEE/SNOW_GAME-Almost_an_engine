#pragma once
#include "HUDWidget.h"
#include "RectangleWidget.h"
#include "TextWidget.h"
#include "Sound.h"
//#include <functional>
class Button:public HUDWidget
{
public:
	Button()
	{
     m_button_hover_sound->createSound(L"Assets\\Sounds\\Button_Hover.wav");
	}
	~Button()
	{

	}
	virtual void Tick(float DeltaTime) override
	{
		HUDWidget::Tick(DeltaTime);

        
	}
	virtual void BeginPlay() override
	{
		HUDWidget::BeginPlay();
		

	}
	void CheckHoverAndPress(const Point cursor_position, bool L_button_pressed,TransformComponent2D button_transform)
	{
		if(m_visible)
		{ 
			m_button_rect->SetVisibility(true);
			m_button_text->SetVisibility(true);
		if (cursor_position.m_x > (button_transform.position.m_x - button_transform.scale.m_x / 2) && 
			cursor_position.m_x < (button_transform.position.m_x + button_transform.scale.m_x / 2) &&
			cursor_position.m_y >(button_transform.position.m_y - button_transform.scale.m_y / 2) && 
			cursor_position.m_y < (button_transform.position.m_y + button_transform.scale.m_y / 2)
			) ButtonHovered();
		else ButtonNOTHovered();

		if (L_button_pressed && !m_is_hovered) m_button_active = false;

        
		if (m_is_hovered && L_button_pressed)ButtonPressed();
		else ButtonReleased();

	    }
		else
		{
			m_button_rect->SetVisibility(false);
			m_button_text->SetVisibility(false);

		}
	

	}

	void UpdatePosition(Point position)
	{
		SetPosition(position);
		auto& transform = m_EntityComponentSystemManager->GetComponent<TransformComponent2D>(m_ID);
		auto& transform_text = m_EntityComponentSystemManager->GetComponent<TransformComponent2D>(m_button_text->GetEntityID());
		auto& transform_rect = m_EntityComponentSystemManager->GetComponent<TransformComponent2D>(m_button_rect->GetEntityID());

		transform_text.position = transform.position;
		transform_rect.position = transform.position;


	}

	void SetDefaultRectColor(Vector3D color)
	{
		m_default_rect_color = color;
	}
	
	void SetFnc(void(*fnc)())
	{
		m_fnc = fnc;
	}

	RectangleWidget* GetButtonRectangle()
	{
		return m_button_rect;
	}
	TextWidget* GetButtonTextWidget()
	{
		return m_button_text;
	}
	
private:
	bool m_is_hovered = false;
	bool m_is_pressed = false;
	bool m_button_active = true;
	bool m_hov_funcs = true;
	RectangleWidget* m_button_rect = new RectangleWidget;
	TextWidget* m_button_text = new TextWidget;
	Vector3D m_default_rect_color ;
	void(*m_fnc)()= nullptr;
	Sound* m_button_hover_sound = new Sound;

	void ButtonPressed()
	{
		m_is_pressed = true;
	  
		if(m_button_active)
		{ 
		m_fnc();
		m_button_rect->SetBrushColor(m_default_rect_color* 0.5);
        }
		m_button_active = false;
	}
	void ButtonReleased()
	{
		m_is_pressed = false;
		m_button_active = true;
	}
	void ButtonHovered()
	{
		m_is_hovered = true;
		m_button_rect->SetBrushColor(m_default_rect_color * 0.8f);
		
		if (m_hov_funcs)
		{
			m_button_hover_sound->playSound(2);

		}

		m_hov_funcs = false;
	}
	void ButtonNOTHovered()
	{
		m_is_hovered = false;
		m_button_rect->SetBrushColor(m_default_rect_color);
		
		m_hov_funcs = true;
	}


};
