#pragma once
#include <d3d11.h>
#include "Prerequisites.h"


class IndexBuffer
{
public:
	IndexBuffer(void* list_indices, UINT size_list,RenderSystem* render_system);

	UINT getSizeIndexList();
	
	~IndexBuffer();

private:
	
	UINT m_size_list;

	ID3D11Buffer* m_buffer;
	RenderSystem* m_render_system = nullptr;


	friend class DeviceContext;

};


