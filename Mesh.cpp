//The MIT License(MIT)
//
//Copyright(c) 2012 - 2019 Syoyo Fujita and many contributors.
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this softwareand associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright noticeand this permission notice shall be included in
//all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//THE SOFTWARE.
#include "Mesh.h"
#include <Windows.h>

#define TINYOBJLOADER_IMPLEMENTATION


#include <tiny_obj_loader.h>



#include <locale>
#include <codecvt>

#include "GraphicsEngine.h"
#include "VertexMesh.h"


//#define FBXSDK_NEW_API
//#include "fbxsdk.h"


Mesh::Mesh()
{

}

Mesh::Mesh(const wchar_t* full_path):Resource(full_path)
{
	tinyobj::attrib_t attributes;
	std::vector<tinyobj::shape_t>shapes;
	std::vector<tinyobj::material_t>materials;

	std::string warn;
	std::string err;
	 
	std::string inputfile = utf8_encode(full_path);

	std::string mtldir = inputfile.substr(0,inputfile.find_last_of("\\/"));

	//Load the obj file
	bool res = tinyobj::LoadObj(&attributes, &shapes, &materials, &warn, &err, inputfile.c_str(), mtldir.c_str());
	
	if (!err.empty())
	{
		throw std::exception("Mesh was not created successfully");
	}
	if (!res)
	{
		throw std::exception("Mesh was not created successfully");
	}


	std::vector<VertexMesh>list_vertices;
	std::vector<unsigned int>list_indices;

	size_t size_vertex_index_list = 0;

	for (size_t s = 0; s < shapes.size(); s++)//set size 
	{
		size_vertex_index_list += shapes[s].mesh.indices.size();
	}

    list_vertices.reserve(size_vertex_index_list);
	list_indices.reserve(size_vertex_index_list);

	m_material_slots.resize(materials.size());

	size_t index_global_offset = 0;

	for (size_t m = 0; m < materials.size(); m++)//run over all meterials
	{
		m_material_slots[m].start_index = index_global_offset;

		m_material_slots[m].material_id = m;

		for (size_t s = 0; s < shapes.size(); s++)//go over all shapes
		{
			size_t index_offset = 0;


			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
			{

				unsigned char num_face_verts = shapes[s].mesh.num_face_vertices[f];

                if (shapes[s].mesh.material_ids[f] != m)
				{
					index_offset += num_face_verts;

					continue;
				}
				Vector3D vertices_face[3];
				Vector2D texcoords_face[3];

				for (unsigned char v = 0; v < num_face_verts; v++)
				{
					tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

					tinyobj::real_t vx = attributes.vertices[index.vertex_index * 3 + 0];
					tinyobj::real_t vy = attributes.vertices[index.vertex_index * 3 + 1];
					tinyobj::real_t vz = attributes.vertices[index.vertex_index * 3 + 2];

					tinyobj::real_t tx = 0;
					tinyobj::real_t ty = 0;
					if (attributes.texcoords.size())
					{
						tx = attributes.texcoords[index.texcoord_index * 2 + 0];
						ty = attributes.texcoords[index.texcoord_index * 2 + 1];
					}
					vertices_face[v] = Vector3D(vx,vy, vz);
					texcoords_face[v] = Vector2D(tx, ty);
				}
				Vector3D tangent, bitangent;
				computeTangents(vertices_face[0], vertices_face[1], vertices_face[2],
					            texcoords_face[0], texcoords_face[1], texcoords_face[2],
					            tangent, bitangent);


				for (unsigned char v = 0; v < num_face_verts; v++)
				{
					tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

					tinyobj::real_t vx = attributes.vertices[index.vertex_index * 3 + 0];
					tinyobj::real_t vy = attributes.vertices[index.vertex_index * 3 + 1];
					tinyobj::real_t vz = attributes.vertices[index.vertex_index * 3 + 2];

					tinyobj::real_t tx = 0;
					tinyobj::real_t ty = 0;
					if(attributes.texcoords.size())
					{ 
					 tx = attributes.texcoords[index.texcoord_index * 2 + 0];
					 ty = attributes.texcoords[index.texcoord_index * 2 + 1];
                    }

					tinyobj::real_t nx = 0;
					tinyobj::real_t ny = 0;
					tinyobj::real_t nz = 0;
					if (attributes.texcoords.size())
					{
						nx = attributes.normals[index.normal_index * 3 + 0];
						ny = attributes.normals[index.normal_index * 3 + 1];
					    nz = attributes.normals[index.normal_index * 3 + 2];
					}
					Vector3D v_tangent, v_bitangent;
					v_bitangent = Vector3D::cross(Vector3D(nx, ny, nz),tangent) ;
					v_tangent = Vector3D::cross(v_bitangent, Vector3D(nx, ny, nz) );

					VertexMesh vertex(Vector3D(vx, vy, vz), Vector2D(tx, ty), Vector3D(nx, ny, nz),v_tangent,v_bitangent);
					list_vertices.push_back(vertex);
					list_indices.push_back((unsigned int)index_global_offset + v);

				}

				index_offset += num_face_verts;
				index_global_offset += num_face_verts;

			}

		}

		m_material_slots[m].num_indices = index_global_offset - m_material_slots[m].start_index;
	}
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::GetGraphicsEngine()->getVertexMeshLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader);
	m_vertex_buffer = GraphicsEngine::GetGraphicsEngine()->getRenderSystem()
	                ->createVertexBuffer(&list_vertices[0],sizeof(VertexMesh), (UINT)list_vertices.size(), shader_byte_code, (UINT)size_shader);

	m_index_buffer = GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->createIndexBuffer(&list_indices[0],(UINT)list_indices.size());
}

Mesh::Mesh(VertexMesh* vertex_list_data, unsigned int vertex_list_size,
	       unsigned int* index_list_data, unsigned int index_list_size, 
	       MaterialSlot* material_slot_list, unsigned int material_slot_list_size) :Resource(L"")
{
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::GetGraphicsEngine()->getVertexMeshLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader);

	m_vertex_buffer = GraphicsEngine::GetGraphicsEngine()->getRenderSystem()
		->createVertexBuffer(vertex_list_data, sizeof(VertexMesh), (UINT)vertex_list_size, shader_byte_code, (UINT)size_shader);

	m_index_buffer = GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->createIndexBuffer(index_list_data, (UINT)index_list_size);

	m_material_slots.resize(material_slot_list_size);

	for (unsigned int i = 0; i < material_slot_list_size; i++)
	{
		m_material_slots[i] = material_slot_list[i];
	}

}

Mesh::~Mesh()
{
}

const VertexBufferPtr& Mesh::getVertexBuffer()
{
	return m_vertex_buffer;
}

const IndexBufferPtr& Mesh::getIndexBuffer()
{
	return m_index_buffer;
}

const MaterialSlot& Mesh::getMaterialSlot(unsigned int slot)
{
	if (slot >= m_material_slots.size()) return MaterialSlot();
	return m_material_slots[slot];

}

size_t Mesh::getNumMaterialSlots()
{
	return m_material_slots.size();
}

void Mesh::computeTangents(const Vector3D& v0, const Vector3D& v1, const Vector3D& v2,
	                       const Vector2D& t0, const Vector2D& t1, const Vector2D& t2,
	                       Vector3D& tangent, Vector3D& bitangent)
{
	Vector3D deltaPos1 = v1 - v0;
	Vector3D deltaPos2 = v2 - v0;

	Vector2D deltaUV1 = t1 - t0;
	Vector2D deltaUV2 = t2 - t0;

	float r = 1.0f / (deltaUV1.m_x * deltaUV2.m_y - deltaUV1.m_y * deltaUV2.m_x);
	tangent = (deltaPos1 * deltaUV2.m_y - deltaPos2 * deltaUV1.m_y);
	tangent = Vector3D::normalize(tangent);
	bitangent = (deltaPos2 * deltaUV1.m_x - deltaPos1 * deltaUV2.m_x);
	bitangent = Vector3D::normalize(bitangent);



}

std::string Mesh::utf8_encode(const std::wstring& wstr)
{
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}
