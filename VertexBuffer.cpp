#include "VertexBuffer.h"
#include "RenderSystem.h"
#include <exception>


VertexBuffer::VertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list,
void* shader_byte_code, size_t size_byte_shader,RenderSystem* render_system) : m_render_system(render_system),m_layout(0), m_buffer(0)
{
	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = size_vertex * size_list;
	buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = list_vertices;

	m_size_vertex = size_vertex;
	m_size_list = size_list;

	if (FAILED(m_render_system->m_d3d_device->CreateBuffer(&buff_desc, &init_data, &m_buffer)))
	{
		throw std::exception("VertexBuffer was not created successfully");
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION"     ,//SEMANTIC NAME
		0        ,//SEMANTIC INDEX
		DXGI_FORMAT_R32G32B32_FLOAT,//FORMAT
		0        ,//INPUT SLOT
		0        ,//ALIGNED BYTE OFFSET
		D3D11_INPUT_PER_VERTEX_DATA,//INPUT SLOT CLASS
		0          //INSTANCE DATA STEP RATE
		},

		{"TEXCOORD"  ,//SEMANTIC NAME
		0      ,//SEMANTIC INDEX
		DXGI_FORMAT_R32G32_FLOAT,//FORMAT
		0  ,//INPUT SLOT
		12 ,//ALIGNED BYTE OFFSET
		D3D11_INPUT_PER_VERTEX_DATA,//INPUT SLOT CLASS
		0    //INSTANCE DATA STEP RATE
		},

		{"NORMAL"     ,//SEMANTIC NAME
		0        ,//SEMANTIC INDEX
		DXGI_FORMAT_R32G32B32_FLOAT,//FORMAT
		0        ,//INPUT SLOT
		20        ,//ALIGNED BYTE OFFSET
		D3D11_INPUT_PER_VERTEX_DATA,//INPUT SLOT CLASS
		0          //INSTANCE DATA STEP RATE
		},

		{"TANGENT"     ,//SEMANTIC NAME
		0        ,//SEMANTIC INDEX
		DXGI_FORMAT_R32G32B32_FLOAT,//FORMAT
		0        ,//INPUT SLOT
		32        ,//ALIGNED BYTE OFFSET
		D3D11_INPUT_PER_VERTEX_DATA,//INPUT SLOT CLASS
		0          //INSTANCE DATA STEP RATE
		},

		{"BINORMAL"     ,//SEMANTIC NAME
		0        ,//SEMANTIC INDEX
		DXGI_FORMAT_R32G32B32_FLOAT,//FORMAT
		0        ,//INPUT SLOT
		44        ,//ALIGNED BYTE OFFSET
		D3D11_INPUT_PER_VERTEX_DATA,//INPUT SLOT CLASS
		0          //INSTANCE DATA STEP RATE
		}



	};

	UINT size_layout = ARRAYSIZE(layout);

	if (FAILED(m_render_system->m_d3d_device->CreateInputLayout(layout, size_layout, shader_byte_code, size_byte_shader, &m_layout)))
	{

		throw std::exception("InputLayout was not created successfully");

	}


}


UINT VertexBuffer::getSizeVertexList()
{
	return this->m_size_list;
}

VertexBuffer::~VertexBuffer()
{
	m_layout->Release();
	m_buffer->Release();
}
