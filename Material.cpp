#include "Material.h"
#include "GraphicsEngine.h"
#include <exception>
#include <stdexcept>



Material::Material()
{
}

Material::Material(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path)
{
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	//PointLight
	GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->compileVertexShaders(vertex_shader_path, "vsmain", &shader_byte_code, &size_shader);
	m_vertex_shader = GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->releaseCompiledShader();

	if (!m_vertex_shader) throw std::runtime_error("Material was not created successfully");

	GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->compilePixelShaders(pixel_shader_path, "psmain", &shader_byte_code, &size_shader);
	m_pixel_shader = GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->releaseCompiledShader();

	if (!m_pixel_shader) throw std::runtime_error("Material was not created successfully");

}

Material::Material(const MaterialPtr& material)
{
	m_vertex_shader = material->m_vertex_shader;
	m_pixel_shader = material->m_pixel_shader;
}

Material::~Material()
{

}

void Material::addTexture(const TexturePtr& texture)
{
	m_vec_textures.push_back(texture);
}

void Material::removeTexture(unsigned int index)
{
	if (index >= this->m_vec_textures.size())return;
	m_vec_textures.erase(m_vec_textures.begin() + index);

}

void Material::setData(void* data, unsigned int size)
{
	if (!m_constant_buffer)
		m_constant_buffer = GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->createConstantBuffer(data, size);
	else
		m_constant_buffer->update(GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->getImmediateDeviceContext(), data);
}

void Material::setCullMode(CULL_MODE cull_mode)
{
	m_cull_mode = cull_mode;

}

CULL_MODE Material::getCullMode()
{
	return m_cull_mode;
}

void Material::updateModel(Vector3D position, Rotator3D rotation, Vector3D scale, const std::vector<MaterialPtr>& list_material, constant constant_object)
{
	constant constantObject;

	Matrix4x4 temp;
    constantObject.m_world.setIdentity();


	temp.setIdentity();
	temp.setScale(scale);
	constantObject.m_world *= temp;

	temp.setIdentity();
	temp.setRotationRadiansX(rotation.GetRadianValue().m_x);
	constantObject.m_world *= temp;

	temp.setIdentity();
	temp.setRotationRadiansY(rotation.GetRadianValue().m_y);
	constantObject.m_world *= temp;

	temp.setIdentity();
	temp.setRotationRadiansZ(rotation.GetRadianValue().m_z);
	constantObject.m_world *= temp;

	temp.setIdentity();
	temp.setTranslation(position);
	constantObject.m_world *= temp;
	constantObject.m_world.setTranslation(position);

	//Pass the constant data to the material
	constantObject.m_view = constant_object.m_view;
	constantObject.m_proj = constant_object.m_proj;
	constantObject.m_camera_position = constant_object.m_camera_position;
	constantObject.m_light_position = constant_object.m_light_position;
	constantObject.m_light_radius = 0.0f;
	constantObject.m_light_direction = constant_object.m_light_direction;
	constantObject.m_time = constant_object.m_time;
	constantObject.m_alpha = constant_object.m_alpha;
	for (size_t m = 0; m < list_material.size(); m++)
	{
		list_material[m]->setData(&constantObject, sizeof(constant));
	}
}

