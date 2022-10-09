#include "HUD.h"
#include<iostream>
//#include "wchar.h"

HUD* HUD::m_system = nullptr;

HUD::HUD()
{
 
    try
    {
       D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

    }
    catch (...)
    {
        throw std::exception("D2D1 Factory was not created successfully");
    }
    try
    {

            DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&pDWriteFactory)
        );
    }
    catch (...)
    {
        throw std::exception("DWrite Factory was not created successfully");
       
    }


}

HUD::~HUD()
{
    SafeRelease(&m_pDirect2dFactory);
    SafeRelease(&m_pRenderTarget);
   

}

 
HRESULT HUD::CreateDeviceResources()
{
    HRESULT hr = S_OK;
    
          

    return hr;
}

HRESULT HUD::BindSurfaceToHUDRT(IDXGISurface* surface)
{
    HRESULT hr = S_OK;


    D2D1_RENDER_TARGET_PROPERTIES props =
        D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
            96,
            96
        );

 //  if (m_pDirect2dFactory != nullptr)std::cout << "ok";
  //  else  std::cout << "nawt gord";
     hr = m_pDirect2dFactory
        ->CreateDxgiSurfaceRenderTarget(
            surface,
            &props,
            &m_pRenderTarget
                                      );

    /* if (SUCCEEDED(hr))
     hr = CreateDeviceResources();*/
 if (SUCCEEDED(hr))
     hr = CreateDeviceResources();


    return hr;
}

void HUD::ClearHUD(Vector3D color, float alpha)
{
    
   
        m_pRenderTarget->BeginDraw();
 
        m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

        m_pRenderTarget->Clear( D2D1::ColorF(color.m_x, color.m_y, color.m_z,alpha));
 
        m_pRenderTarget->EndDraw();
    
  
}

void HUD::DrawTextOnScreen(D2D1_POINT_2F position,IDWriteTextLayout* text_layout, ID2D1SolidColorBrush* text_brush)
{
      
  m_pRenderTarget->BeginDraw();


  if (text_layout != NULL && text_brush != NULL)
    m_pRenderTarget->DrawTextLayout(
        position,
        text_layout,
        text_brush
    );

    m_pRenderTarget->EndDraw();

}
//
//void HUD::DrawNumberOnScreen(int number, Point position, float width, float height, float font_size, Vector3D text_color)
//{
//   
//    wchar_t string[10];
//    _snwprintf_s(string, sizeof(string), L"%d", number);
//  //  m_pRenderTarget->BeginDraw();
//
//  //  DrawTextOnScreen(string, position, width, height, font_size, text_color);
//
//  //  m_pRenderTarget->EndDraw();
//
//}

//void HUD::DrawRectangle(Point position, Point size, Vector3D rectangle_color, WidgetPositionOffset offset ,
//                        ShapeType shape_type , float border_width , Vector3D border_color )
void HUD::DrawRectangle(Point position, Point size, ID2D1SolidColorBrush* rectangle_brush, WidgetPositionOffset offset,
    ShapeType shape_type, float border_width, ID2D1SolidColorBrush* border_brush)
{
   
    D2D1_RECT_F rectangle1;

    rectangle1 = D2D1::RectF(
        FLOAT(position.m_x - size.m_x/2),
        FLOAT(position.m_y - size.m_y/2),
        FLOAT(position.m_x + size.m_x/2),
        FLOAT(position.m_y + size.m_y/2)
    );

    if(offset == WidgetPositionOffset::Right)
    rectangle1 = D2D1::RectF(
        FLOAT(position.m_x),
        FLOAT(position.m_y - size.m_y / 2),
        FLOAT(position.m_x + size.m_x),
        FLOAT(position.m_y + size.m_y / 2)
    );
    else if (offset == WidgetPositionOffset::Left)
        rectangle1 = D2D1::RectF(
            FLOAT(position.m_x - size.m_x),
            FLOAT(position.m_y - size.m_y / 2),
            FLOAT(position.m_x) ,
            FLOAT(position.m_y + size.m_y / 2)
        );

    m_pRenderTarget->BeginDraw();


    // Draw a filled rectangle.
    if (shape_type == ShapeType::Filled)
    {
        if (rectangle_brush != NULL) 
            m_pRenderTarget->FillRectangle(&rectangle1, rectangle_brush);
       
    }
    else if (shape_type == ShapeType::Empty)
    {
        if (rectangle_brush != NULL)
             m_pRenderTarget->DrawRectangle(&rectangle1, rectangle_brush,border_width);

    }

    else if (shape_type == ShapeType::Filled_Borders)
    {
       
 
        if (rectangle_brush != NULL)
            m_pRenderTarget->FillRectangle(&rectangle1, rectangle_brush);

        if (rectangle_brush != NULL )
            m_pRenderTarget->DrawRectangle(&rectangle1, border_brush, border_width);



    }



    m_pRenderTarget->EndDraw();


}

ID2D1RenderTarget* HUD::GetRenderTarget()
{
    return m_pRenderTarget;
}

ID2D1Factory* HUD::GetD2D1Factory()
{
    return m_pDirect2dFactory;
}

IDWriteFactory* HUD::GetWriteFactory()
{

    return pDWriteFactory;
}

RECT HUD::GetScreenRect()
{
    return m_screen_rect;
}

void HUD::setScreenRect(RECT rect)
{
    m_screen_rect = rect;
}

HUD* HUD::GetHUDSystem()
{
    return m_system;
}

void HUD::create()
{
    if (HUD::m_system)
        throw std::exception("HUD System already created");
    HUD::m_system = new HUD();
}

void HUD::release()
{
    if (!HUD::m_system)
        return;
    delete HUD::m_system;
   
}
