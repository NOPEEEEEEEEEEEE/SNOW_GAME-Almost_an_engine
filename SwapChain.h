#pragma once
#include <d3d11.h>
#include "Prerequisites.h"


class SwapChain
{
public:

    //Initialize the SwapChain for a window 
	SwapChain(HWND hwnd,UINT width,UINT height,RenderSystem* render_system);
	
	void setFullScreen(bool fullscreen,unsigned int width, unsigned int height);


	void resize(unsigned int width, unsigned int height);
	bool present(bool vsync);

	//Release the SwapChain
	~SwapChain();

private:
	void reloadBuffers(unsigned int width, unsigned int height);

	IDXGISwapChain* m_swap_chain = nullptr;
	ID3D11RenderTargetView* m_render_target_view = nullptr;
	ID3D11RenderTargetView* m_HUD_render_target_view = nullptr;
	ID3D11DepthStencilView* m_depth_stencil_view = nullptr;
	RenderSystem* m_render_system = nullptr;


	friend class DeviceContext;
	friend class HUD;
};
