#include "PixelShader.h"
#include "RenderSystem.h"
#include <exception>


PixelShader::PixelShader(const void* shader_byte_code, size_t byte_code_size,RenderSystem* render_system) : m_render_system(render_system)
{

if (FAILED(m_render_system->m_d3d_device->CreatePixelShader(shader_byte_code, byte_code_size, nullptr, &m_pixel_shader)))
     throw std::exception("PixelShader was not created successfully");

}


PixelShader::~PixelShader()
{
	m_pixel_shader->Release();

}

