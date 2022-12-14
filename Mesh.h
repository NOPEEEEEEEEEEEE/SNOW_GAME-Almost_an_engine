
#pragma once
#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Vector3D.h"
#include "Vector2D.h"
#include <vector>
#include "VertexMesh.h"

struct MaterialSlot
{
	size_t start_index = 0;
	size_t num_indices = 0;
	size_t material_id = 0;
};

class Mesh:public Resource
{
public:
	Mesh();
	Mesh(const wchar_t* full_path);
	Mesh(VertexMesh* vertex_list_data, unsigned int vertex_list_size,
		unsigned int* index_list_data, unsigned int index_list_size,
		MaterialSlot* material_slot_list, unsigned int material_slot_list_size);

	~Mesh();
	const VertexBufferPtr& getVertexBuffer();
	const IndexBufferPtr& getIndexBuffer();

	const MaterialSlot& getMaterialSlot(unsigned int slot);
	size_t getNumMaterialSlots();

private:

	void computeTangents(const Vector3D& v0, const Vector3D& v1, const Vector3D& v2,
		                 const Vector2D& t0, const Vector2D& t1, const Vector2D& t2,
		                       Vector3D& tangent, Vector3D& binormal);


	VertexBufferPtr m_vertex_buffer;
	IndexBufferPtr m_index_buffer;
	std::vector<MaterialSlot>m_material_slots;

	std::string utf8_encode(const std::wstring& wstr);

	friend class DeviceContext;
};