#pragma once
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include <vector>
#include "Vector4D.h"
#include "Rotator3D.h"
#include "Matrix4x4.h"

enum class CULL_MODE
{
	CULL_MODE_FRONT = 0,
	CULL_MODE_BACK,
	CULL_MODE_NONE
};

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	Vector4D m_light_direction;
	Vector4D m_camera_position;
	Vector4D m_light_position = Vector4D(0, 1.0, 0, 0);
	float m_light_radius = 4.0f;
	float m_time = 0.0f;
	float m_alpha = 1;

};

class GraphicsEngine;
class Material
{
public:
	Material();
	Material(const wchar_t* vertex_shader_path,const wchar_t* pixel_shader_path);
	Material(const MaterialPtr& material);
   ~Material();

   void addTexture(const TexturePtr& texture);
   void removeTexture(unsigned int index);
  
   void setData(void* data, unsigned int size);

   void setCullMode(CULL_MODE cull_mode);
   CULL_MODE getCullMode();

   static  void updateModel(Vector3D position, Rotator3D rotation, Vector3D scale, const std::vector<MaterialPtr>& list_material,constant constant_object);

private:
	VertexShaderPtr m_vertex_shader;
	PixelShaderPtr m_pixel_shader;
	ConstantBufferPtr m_constant_buffer;
	std::vector<TexturePtr>m_vec_textures;

	CULL_MODE m_cull_mode = CULL_MODE::CULL_MODE_BACK;

	friend class GraphicsEngine;

};

