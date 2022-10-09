#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class ConstantBuffer
{
public:
	ConstantBuffer(void* buffer, UINT size_buffer,RenderSystem* render_system);
	
	void update(DeviceContextPtr context,void* buffer);
	
	~ConstantBuffer();

private:
	ID3D11Buffer* m_buffer;
	RenderSystem* m_render_system = nullptr;


	friend class DeviceContext;

};