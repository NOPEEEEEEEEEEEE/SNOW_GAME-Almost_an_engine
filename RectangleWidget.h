#pragma once
#include "HUDWidget.h"

class RectangleWidget :public HUDWidget
{
public:
	
	RectangleWidget()
	{
		
	}
	~RectangleWidget()
	{
		 
	}
	virtual void Tick(float DeltaTime) override
	{
		HUDWidget::Tick(DeltaTime);
		if (!m_visible)return;
		auto& transform = m_EntityComponentSystemManager->GetComponent<TransformComponent2D>(m_ID);
		
	     // std::cout<<m_Brush->GetColor().a;
		//std::cout << transform.scale.m_x;

		HUD::GetHUDSystem()->DrawRectangle(transform.position, transform.scale, m_Brush,m_offset, m_rect_type, m_border_width, m_border_Brush);
		//std::cout << "ok";

	}
	virtual void BeginPlay() override
	{
		HUDWidget::BeginPlay();
		


	}
	void CreateColor(Vector3D brush_color, Vector3D border_brush_color = Vector3D(0,0,0))
	{
		D2D1_COLOR_F color = D2D1::ColorF(brush_color.m_x, brush_color.m_y, brush_color.m_z, 1);
		D2D1_COLOR_F border_color = D2D1::ColorF(border_brush_color.m_x, border_brush_color.m_y, border_brush_color.m_z, 1);

	//	HUD_Render_System->CreateBrush(m_Brush) ;
		HUD::GetHUDSystem()->GetRenderTarget()->CreateSolidColorBrush(
			color,
			&m_Brush
		);
		HUD::GetHUDSystem()->GetRenderTarget()->CreateSolidColorBrush(
			border_color,
			&m_border_Brush
		);

		
	}
	void SetBrushColor(Vector3D brush_color)
	{
		D2D1_COLOR_F color = D2D1::ColorF(brush_color.m_x, brush_color.m_y, brush_color.m_z, 1);
		m_Brush->SetColor(color);

	}
	ID2D1SolidColorBrush* GetBrush()
	{
		return m_Brush;
	}

	void SetRectType(ShapeType rect_type)
	{
		m_rect_type = rect_type;

	}
	void SetBorderWidth(float width)
	{
		m_border_width = width;
	}
	
private:

	ID2D1SolidColorBrush* m_Brush= NULL ;
	ID2D1SolidColorBrush* m_border_Brush = NULL;
	float m_border_width = 0;

	ShapeType m_rect_type = ShapeType::Filled;
	//friend class WidgetManager;

};
