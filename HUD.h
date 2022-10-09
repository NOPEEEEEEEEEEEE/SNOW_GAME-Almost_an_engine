#pragma once
#include "Window.h"

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#include "Vector3D.h"
#include "Point.h"
//#include "RectangleWidget.h"
//#include "HUDWidget.h"


template<class Interface>
inline void SafeRelease(
	Interface** ppInterfaceToRelease
)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}


//enum class ShapeType
//{
//	Filled,
//	Empty,
//	Filled_Borders
//
//};
enum class ShapeType
{
	Filled,
	Empty,
	Filled_Borders

};

enum class WidgetPositionOffset
{
	Center,
	Right,
	Left

};

class HUD  //: public System
{
public:
	

   //HRESULT Initialize();

  // void RunMessageLoop();

   // Inherited via Window
 // void Initialize(RECT screen_rect) ;

  void ClearHUD(Vector3D color, float alpha);

  void DrawTextOnScreen(D2D1_POINT_2F position,IDWriteTextLayout* text_layout, ID2D1SolidColorBrush* text_brush);

 // void DrawNumberOnScreen(int number, Point position, float width, float height, float font_size, Vector3D text_color);

 /* void DrawRectangle(Point position,Point size,Vector3D rectangle_color,
	       WidgetPositionOffset offset = WidgetPositionOffset::Center , ShapeType shape_type= ShapeType::Filled,float border_width = 1, Vector3D border_color = Vector3D(0,0,0));*/
  void DrawRectangle(Point position, Point size, ID2D1SolidColorBrush* rectangle_brush,
	         WidgetPositionOffset offset = WidgetPositionOffset::Center, ShapeType shape_type = ShapeType::Filled,
	         float border_width = 1, ID2D1SolidColorBrush* border_brush = NULL);


 
  HRESULT BindSurfaceToHUDRT(IDXGISurface* surface);

 // void Release() ;

  // virtual void OnFocus() override;

 // void Resize(RECT screen_rect);

   //virtual void OnKillFocus() override;

  ID2D1RenderTarget* GetRenderTarget();
  ID2D1Factory* GetD2D1Factory();
  IDWriteFactory* GetWriteFactory(); 
  RECT GetScreenRect();
  void setScreenRect(RECT rect);

  ///////////////HUD SYSTEM
  static HUD* GetHUDSystem();
  static void create();
  static void release();

private:

    HUD();
   ~HUD();

//	HRESULT CreateDeviceIndependentResources();

	HRESULT CreateDeviceResources();

	RECT m_screen_rect{};

	ID2D1RenderTarget* m_pRenderTarget;

	static HUD* m_system;
	

	ID2D1Factory* m_pDirect2dFactory = nullptr ;
    IDWriteFactory* pDWriteFactory = nullptr;
	

	//friend class HUDWidget;
};

