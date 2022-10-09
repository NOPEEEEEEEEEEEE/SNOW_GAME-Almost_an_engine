#pragma once
#include "HUDWidget.h"

class TextWidget :public HUDWidget
{
public:

	TextWidget()
	{

	}
	~TextWidget()
	{

	}
	virtual void Tick(float DeltaTime) override
	{
		HUDWidget::Tick(DeltaTime);
		if (!m_visible)return;
		auto& transform = m_EntityComponentSystemManager->GetComponent<TransformComponent2D>(m_ID);
		//std::cout << m_Brush->GetColor().a;
		//std::cout << transform.scale.m_x;


		D2D1_POINT_2F origin = D2D1::Point2F(
			static_cast<FLOAT>(transform.position.m_x- transform.scale.m_x / 2),
			static_cast<FLOAT>(transform.position.m_y- transform.scale.m_y / 2)
		);

		HUD::GetHUDSystem()->DrawTextOnScreen( origin, m_text_layout,  m_Brush);
		
		//std::cout << "ok";

	}
	virtual void BeginPlay() override
	{
		HUDWidget::BeginPlay();
		


	}
	void SetText(const wchar_t* text )
	{
		m_text = nullptr;
		m_text = text;
		//CreateTextFont();

	}
	void UpdateText(const wchar_t* text)
	{

		m_text = nullptr;
		m_text = text;
		CreateTextFont();
		
	}
	void UpdateText(int number)
	{
		wchar_t string[10];
	    _snwprintf_s(string, sizeof(string), L"%d", number);

		m_text = nullptr;
		m_text = string;

		CreateTextFont();
		
	}

	void SetFontSize(float font_size)
	{
		m_font_size = font_size;
	}
	void CreateColor(Vector3D brush_color)
	{
		D2D1_COLOR_F color = D2D1::ColorF(brush_color.m_x, brush_color.m_y, brush_color.m_z, 1);

		
		HUD::GetHUDSystem()->GetRenderTarget()->CreateSolidColorBrush(
			color,
			&m_Brush
		);

	}
	void UpdateColor(Vector3D brush_color)
	{
		D2D1_COLOR_F color = D2D1::ColorF(brush_color.m_x, brush_color.m_y, brush_color.m_z, 1);

		HUD::GetHUDSystem()->GetRenderTarget()->CreateSolidColorBrush(
			color,
			&m_Brush
		);
		CreateTextFont();
	}

	void CreateTextFont()
	{
		
		m_text_format = NULL;
		HUD::GetHUDSystem()->GetWriteFactory()->CreateTextFormat(
			L"Bauhaus 93",                // Font  name.
			NULL,
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			m_font_size,                 //Font size
			L"en-us",
			&m_text_format
		);

		FLOAT dpiX, dpiY;
	
		dpiX = (FLOAT)GetDpiForWindow(GetDesktopWindow());
		dpiY = dpiX;
		auto& transform = m_EntityComponentSystemManager->GetComponent<TransformComponent2D>(m_ID);

		transform.scale.m_x = transform.scale.m_x + int(HUD::GetHUDSystem()->GetScreenRect().right / dpiX);
		transform.scale.m_y = transform.scale.m_y + int(HUD::GetHUDSystem()->GetScreenRect().bottom / dpiY);

		m_text_format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

		m_text_format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
        UINT32 textLength;
		textLength = (UINT32)wcslen(m_text);
		if (m_text_format != NULL)
			HUD::GetHUDSystem()->GetWriteFactory()->CreateTextLayout(
				m_text,      // The string to be laid out and formatted.
				textLength,  // The length of the string.
				m_text_format,  // The text format to apply to the string 
				FLOAT(transform.scale.m_x),         // The width of the layout box.
				FLOAT(transform.scale.m_y),        // The height of the layout box.
				&m_text_layout  // The IDWriteTextLayout interface pointer.
			);

	}

private:

    const wchar_t* m_text = nullptr;
	float m_font_size = 0;
	ID2D1SolidColorBrush* m_Brush = NULL;
	IDWriteTextFormat* m_text_format{};
	IDWriteTextLayout* m_text_layout{};

};
