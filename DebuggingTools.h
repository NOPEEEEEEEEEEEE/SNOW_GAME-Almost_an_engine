#pragma once

#include "DeviceContext.h"
#include "RenderSystem.h"
#include "GraphicsEngine.h"
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include "Mesh.h"
#include <iostream>

__declspec(align(16))
struct DebugToolsConstant
{
	
	Matrix4x4 m_world;
	Matrix4x4 m_view;
    Matrix4x4 m_proj;
    Vector3D m_color = Vector3D(0,0,0);
};


class DebuggingTools
{
public:
	DebuggingTools()
	{
		m_material = GraphicsEngine::GetGraphicsEngine()->createMaterial(L"DebuggerVertexShader.hlsl", L"DebuggerPixelShader.hlsl");
		m_material->setCullMode(CULL_MODE::CULL_MODE_NONE);

	}
	~DebuggingTools()
	{

	}
	void DrawLine(Vector3D point1, Vector3D point2,float size, Vector3D color, Matrix4x4 m_world, Matrix4x4 m_view, Matrix4x4 m_proj)
	{
		MeshPtr m_quad_mesh;
		std::vector<MaterialPtr> m_list_mat;
		//Calculate direction of the line based on the 2 points
		Vector3D LineVector = point2-point1;
		
		//Calculate the angle between the line and the axis
        float angleX = atan2f(LineVector.m_y, LineVector.m_z);
        float angleY = atan2f(LineVector.m_y, LineVector.m_x);
		float angleZ = atan2f(LineVector.m_z, LineVector.m_x);

		Matrix4x4 world_model, temp;
		world_model.setIdentity();
      
		temp.setIdentity();
		temp.setRotationRadiansX(angleX);
		world_model *= temp;

		temp.setIdentity();
		temp.setRotationRadiansY(angleY);
		world_model *= temp;
		temp.setIdentity();
		temp.setRotationRadiansZ(angleZ);
		world_model *= temp;
		
		//Set the vertices of the line based on the direction of the line
		Vector3D upp1 = point1 - world_model.getXDirection() * size - world_model.getZDirection() * size;
		Vector3D down1 = point1 + world_model.getXDirection() * size + world_model.getZDirection() * size;
		Vector3D rightp1 = point1 - world_model.getYDirection() * size;
		Vector3D leftp1 = point1 + world_model.getYDirection() * size;

		Vector3D upp2 = point2 - world_model.getXDirection() * size - world_model.getZDirection() * size;
		Vector3D down2 = point2 + world_model.getXDirection() * size + world_model.getZDirection() * size;
		Vector3D rightp2 = point2 - world_model.getYDirection() * size;
		Vector3D leftp2 = point2 + world_model.getYDirection() * size;

		//Build the vertex
		VertexMesh quad_vertex_list[] =
		{
			VertexMesh(point1,Vector2D(1,1),Vector3D(),Vector3D(),Vector3D()),
			VertexMesh(upp1,Vector2D(1,0),Vector3D(),Vector3D(),Vector3D()),
			VertexMesh(upp2,Vector2D(0,0),Vector3D(),Vector3D(),Vector3D()),
			VertexMesh(point2,Vector2D(0,1),Vector3D(),Vector3D(),Vector3D()),
			VertexMesh(rightp2,Vector2D(),Vector3D(),Vector3D(),Vector3D()),
			VertexMesh(rightp1,Vector2D(),Vector3D(),Vector3D(),Vector3D()),
			VertexMesh(down1,Vector2D(),Vector3D(),Vector3D(),Vector3D()),
			VertexMesh(down2,Vector2D(),Vector3D(),Vector3D(),Vector3D()),
			VertexMesh(leftp2,Vector2D(),Vector3D(),Vector3D(),Vector3D()),
			VertexMesh(leftp1,Vector2D(),Vector3D(),Vector3D(),Vector3D())

		};
		//Set the triangle indexes
		unsigned int quad_index_list[] =
		{
			0,1,2,
			2,3,0,
			3,4,5,
			5,3,0,
			0,6,7,
			7,3,0,
			0,9,8,
			8,3,0

		};

		MaterialSlot quad_mat_slots[] =
		{
			{0,24,0}
		};
		
		m_quad_mesh = GraphicsEngine::GetGraphicsEngine()->getMeshManager()->createMesh(quad_vertex_list, 10, quad_index_list, 24, quad_mat_slots, 1);

		m_list_mat.clear();
		m_list_mat.push_back(m_material);
		
	    updateModel(Vector3D(0,0,0),Rotator3D(0,0,0), Vector3D(1, 1, 1),m_material,color, m_world, m_view, m_proj);

		GraphicsEngine::GetGraphicsEngine()->drawMesh(m_quad_mesh, m_list_mat);
		
	}

	void DrawPoint(Vector3D point, float size, Vector3D color, Matrix4x4 m_world, Matrix4x4 m_view, Matrix4x4 m_proj)
	{
		MeshPtr m_quad_mesh;
		std::vector<MaterialPtr> m_list_mat;
		//Make the quad always rotate towards the camera
        Vector3D point_camera_direction = point - m_world.getTranslation();

        float angleX = -atan2f(point_camera_direction.m_y, sqrt(pow(point_camera_direction.m_z, 2) + pow(point_camera_direction.m_x, 2)));
		float angleY = atan2f(point_camera_direction.m_x, point_camera_direction.m_z);
		

		Matrix4x4 world_model, temp;
		world_model.setIdentity();

		temp.setIdentity();
		temp.setRotationRadiansX(angleX);
		world_model *= temp;

		temp.setIdentity();
		temp.setRotationRadiansY(angleY);
		world_model *= temp;
		//Build the quad
		Vector3D up_right = point + world_model.getXDirection() * size + world_model.getYDirection() * size;
		Vector3D up_left = point - world_model.getXDirection() * size + world_model.getYDirection() * size;
		Vector3D down_right = point + world_model.getXDirection() * size - world_model.getYDirection() * size;
		Vector3D down_left = point - world_model.getXDirection() * size - world_model.getYDirection() * size;

		
		VertexMesh quad_vertex_list[] =
		{
			VertexMesh(down_left,Vector2D(0,1),Vector3D(),Vector3D(),Vector3D()),
			VertexMesh(up_left,Vector2D(0,0),Vector3D(),Vector3D(),Vector3D()),
			VertexMesh(up_right,Vector2D(1,0),Vector3D(),Vector3D(),Vector3D()),
			VertexMesh(down_right,Vector2D(1,1),Vector3D(),Vector3D(),Vector3D()),

		};
	
		unsigned int quad_index_list[] =
		{
			0,1,2,
			2,3,0

		};

		MaterialSlot quad_mat_slots[] =
		{
			{0,6,0}
		};

		m_quad_mesh = GraphicsEngine::GetGraphicsEngine()->getMeshManager()->createMesh(quad_vertex_list, 4, quad_index_list, 6, quad_mat_slots, 1);

	
		m_list_mat.clear();
		m_list_mat.push_back(m_material);


		updateModel(Vector3D(0, 0, 0), Rotator3D(0, 0, 0), Vector3D(1, 1, 1), m_material, color, m_world, m_view, m_proj);

		GraphicsEngine::GetGraphicsEngine()->drawMesh(m_quad_mesh, m_list_mat);



	}
	void updateModel(Vector3D position, Rotator3D rotation, Vector3D scale, MaterialPtr& material, Vector3D m_color, Matrix4x4 m_world, Matrix4x4 m_view, Matrix4x4 m_proj)
	{
		DebugToolsConstant constantObject;

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

		//Pass the constant data to the material 
		constantObject.m_world.setTranslation(position);
		constantObject.m_view = m_view;
		constantObject.m_proj = m_proj;
		constantObject.m_color = m_color;
		material->setData(&constantObject, sizeof(DebugToolsConstant));
		
		
	}

private:
        MaterialPtr m_material;
		
};