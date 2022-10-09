#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
#include "RenderSystem.h"
#include "TextureManager.h"
#include "MeshManager.h"
#include "Material.h"

class GraphicsEngine
{
public:

   //RenderSystem
	RenderSystem* getRenderSystem();

	//TextureManager
	TextureManager* getTextureManager();

	//Mesh
	MeshManager* getMeshManager();
    void getVertexMeshLayoutShaderByteCodeAndSize(void** byte_code,size_t* size);

	//Material
	MaterialPtr createMaterial(const wchar_t* vertex_shader_path, const wchar_t* pixel_shader_path);
	MaterialPtr createMaterial(const MaterialPtr& material);

	void setMaterial(const MaterialPtr& material);
	
	//Mesh
	void drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& list_material);

	//GraphicsEngine
	static GraphicsEngine* GetGraphicsEngine();
	static void create();
	static void release();


	

private:

    //Initialize the Graphics Engine and the DirectX Device
	GraphicsEngine();

	//Release all the resources loaded
	~GraphicsEngine();
	RenderSystem* m_render_system = nullptr;
	TextureManager* m_tex_manager = nullptr;
	MeshManager* m_mesh_manager = nullptr;

	static GraphicsEngine* m_engine;

	unsigned char m_mesh_layout_byte_code[1024];
	size_t m_mesh_layout_size = 0;

};
