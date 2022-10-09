#pragma once
#include <d3d11.h>
#include "Prerequisites.h"


class VertexShader
{
public:
	VertexShader(const void* shader_byte_code, size_t byte_code_size,RenderSystem* render_system);

	~VertexShader();

private:

	ID3D11VertexShader* m_vertex_shader;
	RenderSystem* m_render_system=nullptr;

	friend class RenderSystem;
	friend class DeviceContext;

};

