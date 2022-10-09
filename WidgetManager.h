#pragma once
#include "EntityComponentSystemManager.h"
#include "System.h"
#include "Vector2D.h"
#include "HUDWidget.h"
#include <iostream>
#include "RectangleWidget.h"
#include "TextWidget.h"
#include "Button.h"


//extern EntityComponentSystemManager m_EntityComponentSystemManager;
class RectangleWidget;
class WidgetManager : public System
{
public:
	WidgetManager()
	{
	}
	~WidgetManager()
	{

	}
	void Tick(float DeltaTime)
	{

		for (auto widgets : m_Widgets)
		{
		  widgets->Tick(DeltaTime);
		}
		
	}

	void BeginPlay()
	{
	
		for (auto widgets : m_Widgets)
		{
			widgets->BeginPlay();
		}
		
	}
	void setECS(EntityComponentSystemManager* ECS)
	{
		m_EntityComponentSystemManager = ECS;
	}

	void CheckButtonHoverAndPress(const Point cursor_position,bool L_button_pressed )
	{
		for (auto button : m_Buttons)
		{
			auto& transform = m_EntityComponentSystemManager->GetComponent<TransformComponent2D>(button->GetEntityID());
			button->CheckHoverAndPress(cursor_position, L_button_pressed, transform);

		}
		
	}

	void RegisterRect(RectangleWidget* widget, const char Name[32], Point position, Point scale, WidgetPositionOffset offset, Vector3D color,
		ShapeType rect_type = ShapeType::Filled,float border_width = 1, Vector3D border_color = Vector3D(0,0,0))
	{

	widget->SetEntityID(m_EntityComponentSystemManager->CreateEntity() );
	widget->SetName(Name);
	widget->SetOffset(offset);
    widget->SetRectType(rect_type);
	widget->CreateColor(color, border_color);
	widget->SetBorderWidth(border_width);
	widget->setECS(m_EntityComponentSystemManager);
	

	TransformComponent2D tr;
	tr.position = position;
	tr.scale = scale;
	m_EntityComponentSystemManager->AddComponent(widget->GetEntityID(), tr);

		m_Max_Widgets++;
		int index = m_Max_Widgets - 1;
		m_Widgets.resize(m_Max_Widgets);
		if (m_Max_Widgets != 0)
		{
			m_Widgets[index] = widget;
		//	m_Entity_Index[widget->GetEntityID()] = index;
		}

		//	std::cout << m_Max_Widgets;

	}
	void RegisterText(TextWidget* widget, const char Name[32], const wchar_t* text, Point position, Point scale, Vector3D color, float font_size)
	{
	
		widget->SetEntityID(m_EntityComponentSystemManager->CreateEntity());
		widget->SetName(Name);
		//widget->SetHUDRS(HUDSystem);
		widget->CreateColor(color);
		widget->SetText(text);
		widget->SetFontSize(font_size);
		widget->setECS(m_EntityComponentSystemManager);

		TransformComponent2D tr;
		tr.position = position;
		tr.scale = scale;
		m_EntityComponentSystemManager->AddComponent(widget->GetEntityID(), tr);

		m_Max_Widgets++;
		int index = m_Max_Widgets - 1;
		m_Widgets.resize(m_Max_Widgets);
		if (m_Max_Widgets != 0)
		{
			m_Widgets[index] = widget;
			//m_Entity_Index[widget->GetEntityID()] = index;
		}

		widget->CreateTextFont();
		
	}

	void RegisterButton(Button* button, const char Name[32], void(*fnc)(),Point button_position, Point button_scale,
		       const wchar_t* text, float font_size, Vector3D text_color,Vector3D rect_color, ShapeType shape_type = ShapeType::Filled,
		       float border_width = 1, Vector3D rect_border_color = Vector3D(0,0,0))
	{

		button->SetEntityID(m_EntityComponentSystemManager->CreateEntity());
		button->SetName(Name);
		//button->SetHUDRS(HUDSystem);
		button->SetDefaultRectColor(rect_color);
		button->setECS(m_EntityComponentSystemManager);
		button->SetFnc(fnc);

		RegisterRect(button->GetButtonRectangle(),Name, button_position, button_scale, 
			         WidgetPositionOffset::Center, rect_color, shape_type, border_width, rect_border_color
		             );

		RegisterText(button->GetButtonTextWidget(), Name,text, button_position, button_scale, text_color, font_size);

		TransformComponent2D tr;
		tr.position = button_position;
		tr.scale = button_scale;
		m_EntityComponentSystemManager->AddComponent(button->GetEntityID(), tr);


		m_Max_Buttons++;
		int index = m_Max_Buttons - 1;
		m_Buttons.resize(m_Max_Buttons);
		if (m_Max_Buttons != 0)
		{
			m_Buttons[index] = button;
			
		}


	}

	
private:

	int m_Max_Widgets = 0;
	int m_Max_Buttons = 0;
	std::vector<HUDWidget*> m_Widgets{};
	std::vector<Button*>m_Buttons{};
	EntityComponentSystemManager* m_EntityComponentSystemManager = nullptr;

};