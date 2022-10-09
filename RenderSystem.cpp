#include "RenderSystem.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include <d3dcompiler.h>
#include <exception>


RenderSystem::RenderSystem()
{
	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};

	UINT num_driver_types = ARRAYSIZE(driver_types);

	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0

	};

	UINT num_feature_levels = ARRAYSIZE(feature_levels);

	HRESULT result = 0;


	for (UINT driver_type_index = 0; driver_type_index < num_driver_types;)
	{


	 result = D3D11CreateDevice(m_dxgi_adapter, driver_types[driver_type_index], NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, feature_levels, num_feature_levels, D3D11_SDK_VERSION,
			 &m_d3d_device, &m_feature_level, &m_imm_context); //out params

		if (SUCCEEDED(result))break;

		++driver_type_index;

	}

	if (FAILED(result))
	  throw std::exception("RenderSystem was not created successfully");

	m_imm_device_context = std::make_shared<DeviceContext>(m_imm_context,this);


	m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
	m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
	m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);

	initRasterizerState();
}

RenderSystem::~RenderSystem()
{
	if (m_vsblob)m_vsblob->Release();
	if (m_psblob)m_psblob->Release();

	m_dxgi_device->Release();
	m_dxgi_adapter->Release();
	m_dxgi_factory->Release();

	m_d3d_device->Release();
	
	
}

SwapChainPtr RenderSystem::createSwapChain(HWND hwnd, UINT width, UINT height)
{
	SwapChainPtr swapchain = nullptr;

	try
	{
		swapchain = std::make_shared<SwapChain>(hwnd, width, height, this);
	}
	catch(...){}
	return swapchain;
}


DeviceContextPtr RenderSystem::getImmediateDeviceContext()
{
	return this->m_imm_device_context;
}

ID3D11Device* RenderSystem::getDevice()
{
	return m_d3d_device;
}

VertexBufferPtr RenderSystem::createVertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, size_t size_byte_shader)
{
	VertexBufferPtr vertexbuffer = nullptr;

	try
	{
		vertexbuffer = std::make_shared<VertexBuffer>(list_vertices, size_vertex, size_list, shader_byte_code, size_byte_shader,this);
	}
	catch (...) {}

	return vertexbuffer;


}

ConstantBufferPtr RenderSystem::createConstantBuffer(void* buffer, UINT size_buffer)
{
	ConstantBufferPtr constantbuffer = nullptr;

	try
	{
		constantbuffer = std::make_shared<ConstantBuffer>(buffer, size_buffer,this);
	}
	catch (...) {}

	return constantbuffer;
	
}

IndexBufferPtr RenderSystem::createIndexBuffer(void* list_indices, UINT size_list)
{
	IndexBufferPtr indexbuffer = nullptr;

	try
	{
		indexbuffer = std::make_shared<IndexBuffer>(list_indices, size_list,this);
	}
	catch (...) {}

	return indexbuffer;
}

VertexShaderPtr RenderSystem::createVertexShader(const void* shader_byte_code, size_t byte_code_size)
{
	VertexShaderPtr vertex_shader = nullptr;
	try
	{
		vertex_shader = std::make_shared <VertexShader>(shader_byte_code, byte_code_size, this);
	}
	catch (...) {}


	return vertex_shader;
}

bool RenderSystem::compileVertexShaders(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
	ID3DBlob* error_blob = nullptr;

	if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "vs_5_0", 0, 0, &m_blob, &error_blob)))
	{

		if (error_blob)error_blob->Release();
		return false;
	}

	*shader_byte_code = m_blob->GetBufferPointer();
	*byte_code_size = m_blob->GetBufferSize();


	return true;
}

void RenderSystem::releaseCompiledShader()
{
	if (m_blob)m_blob->Release();

}

PixelShaderPtr RenderSystem::createPixelShader(const void* shader_byte_code, size_t byte_code_size)
{
	PixelShaderPtr pixel_shader = nullptr;
	try
	{
		pixel_shader = std::make_shared<PixelShader>(shader_byte_code, byte_code_size, this);
	}
	catch (...) {}

	return pixel_shader;
}

bool RenderSystem::compilePixelShaders(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
	ID3DBlob* error_blob = nullptr;
	if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "ps_5_0", 0, 0, &m_blob, &error_blob)))
	{

		if (error_blob)error_blob->Release();
		return false;
	}

	*shader_byte_code = m_blob->GetBufferPointer();
	*byte_code_size = m_blob->GetBufferSize();


	return true;
}

void RenderSystem::setRasterizerState(bool cull, bool cull_front)
{
	if(cull)
	{ 
	  if (cull_front)
	  {
		m_imm_context->RSSetState(m_cull_front_state);
	  }
	  else
	  {
		m_imm_context->RSSetState(m_cull_back_state);
	  }
    }
	else
	{
		m_imm_context->RSSetState(m_no_cull_state);

	}


}

void RenderSystem::initRasterizerState()
{
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = D3D11_CULL_FRONT;
	desc.DepthClipEnable = true;
	desc.FillMode = D3D11_FILL_SOLID;
	m_d3d_device->CreateRasterizerState(&desc,&m_cull_front_state);

	desc.CullMode = D3D11_CULL_NONE;
	desc.DepthClipEnable = true;
	desc.FillMode = D3D11_FILL_SOLID;
	m_d3d_device->CreateRasterizerState(&desc, &m_no_cull_state);


	desc.CullMode = D3D11_CULL_BACK;
	m_d3d_device->CreateRasterizerState(&desc,&m_cull_back_state);


}
