#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "RenderSystem.h"
#include <exception>
GraphicsEngine* GraphicsEngine::m_engine = nullptr;

GraphicsEngine::GraphicsEngine()
{
	try
	{
      m_render_system = new RenderSystem();
	}
	catch (...) 
	{ 
		throw std::exception("Render System was not created successfully"); 
	}

	try
	{
		m_tex_manager = new TextureManager();
	}
	catch (...)
	{
		throw std::exception("Texture Manager was not created successfully");
	}

	try
	{
		m_mesh_manager = new MeshManager();
	}
	catch (...)
	{
		throw std::exception("Mesh Manager was not created successfully");
	}

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	m_render_system->compileVertexShaders(L"VertexMeshLayoutShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	::memcpy(m_mesh_layout_byte_code, shader_byte_code, size_shader);
	m_mesh_layout_size = size_shader;
	m_render_system->releaseCompiledShader();

}


RenderSystem* GraphicsEngine::getRenderSystem()
{
	return m_render_system;
}

TextureManager* GraphicsEngine::getTextureManager()
{
	return m_tex_manager;
}

GraphicsEngine::~GraphicsEngine()
{
	GraphicsEngine::m_engine = nullptr;
	delete m_mesh_manager;
	delete m_tex_manager;
	delete m_render_system;
}

GraphicsEngine* GraphicsEngine::GetGraphicsEngine()
{
	return m_engine;
}

void GraphicsEngine::create()
{
	if (GraphicsEngine::m_engine)
		throw std::exception("Graphics Engine already created");
	GraphicsEngine::m_engine = new GraphicsEngine();
}

void GraphicsEngine::release()
{
	if (!GraphicsEngine::m_engine)
		return;
	delete GraphicsEngine::m_engine;
}

MeshManager* GraphicsEngine::getMeshManager()
{
	return m_mesh_manager;
}

void GraphicsEngine::getVertexMeshLayoutShaderByteCodeAndSize(void** byte_code, size_t* size)
{
	*byte_code = m_mesh_layout_byte_code;
	*size = m_mesh_layout_size;
}

MaterialPtr GraphicsEngine::createMaterial(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path)
{
	MaterialPtr material = nullptr;

	try
	{
		material = std::make_shared<Material>(vertex_shader_path, pixel_shader_path);
	}
	catch (...) {}

	return material;

}

MaterialPtr GraphicsEngine::createMaterial(const MaterialPtr& material)
{
	MaterialPtr mat = nullptr;

	try
	{
		mat = std::make_shared<Material>(material);
	}
	catch (...) {}

	return mat;
}

void GraphicsEngine::setMaterial(const MaterialPtr& material)
{
	GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->setRasterizerState(material->m_cull_mode != CULL_MODE::CULL_MODE_NONE,material->m_cull_mode == CULL_MODE::CULL_MODE_FRONT);


	if (material->m_constant_buffer)
	{
		GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->getImmediateDeviceContext()
			->setConstantBuffer(material->m_vertex_shader, material->m_constant_buffer);
		GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->getImmediateDeviceContext()
			->setConstantBuffer(material->m_pixel_shader, material->m_constant_buffer);
	}


	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE 
	GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(material->m_vertex_shader);
	GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(material->m_pixel_shader);

	if(material->m_vec_textures.size())
	{ 
	GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->getImmediateDeviceContext()
		                         ->setTexture(material->m_pixel_shader,&material->m_vec_textures[0], unsigned int(material->m_vec_textures.size()));
    }

}

void GraphicsEngine::drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& list_material)
{
	//SET THE VERTICES OF THE TRIANGLE 
	getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());

	//SET THE INDICES OF THE VERTICES OF THE TRIANGLE 
	getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());


	for (size_t m = 0; m < mesh->getNumMaterialSlots(); m++)
	{

		if (m >= list_material.size())break;

		MaterialSlot mat = mesh->getMaterialSlot(unsigned int(m));

		setMaterial(list_material[m]);

		// DRAW THE TRIANGLE
		getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(UINT(mat.num_indices), 0,UINT( mat.start_index));

	}

}
