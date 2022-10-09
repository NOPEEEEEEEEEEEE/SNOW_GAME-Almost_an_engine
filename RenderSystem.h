#pragma once
#include <d3d11.h>
#include "Prerequisites.h"


class RenderSystem
{
public:

    //Initialize the Graphics Engine and the DirectX Device
	RenderSystem();

	//Release all the resources loaded
	~RenderSystem();

	//Swap Chain
	SwapChainPtr createSwapChain(HWND hwnd, UINT width, UINT height);

	//DeviceContext
	DeviceContextPtr getImmediateDeviceContext();

	//Device
	ID3D11Device* getDevice();

	//Vertex buffer
	VertexBufferPtr createVertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, size_t size_byte_shader);

	//Constant buffer
	ConstantBufferPtr createConstantBuffer(void* buffer, UINT size_buffer);

	//IndexBuffer
	IndexBufferPtr createIndexBuffer(void* list_indices, UINT size_list);

	//Shaders
	   //VERTEX SHADER
	VertexShaderPtr createVertexShader(const void* shader_byte_code, size_t byte_code_size);
	bool compileVertexShaders(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
	void releaseCompiledShader();


	  //PIXEL SHADER
	PixelShaderPtr createPixelShader(const void* shader_byte_code, size_t byte_code_size);
	bool compilePixelShaders(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);


	void setRasterizerState(bool cull ,bool cull_front);
	

private:
	void initRasterizerState();

	DeviceContextPtr m_imm_device_context;

	ID3D11Device* m_d3d_device;
	D3D_FEATURE_LEVEL m_feature_level;

	IDXGIDevice* m_dxgi_device = NULL;
	IDXGIAdapter* m_dxgi_adapter = NULL;
	IDXGIFactory* m_dxgi_factory = NULL;
	

	ID3D11DeviceContext* m_imm_context;
	ID3DBlob* m_blob = nullptr;

	ID3D11RasterizerState* m_cull_front_state = nullptr;
	ID3D11RasterizerState* m_cull_back_state = nullptr;
	ID3D11RasterizerState* m_no_cull_state = nullptr;


	ID3DBlob* m_vsblob = nullptr;
	ID3DBlob* m_psblob = nullptr;
	ID3D11VertexShader* m_vertex_shader = nullptr;
	ID3D11PixelShader* m_pixel_shader = nullptr;


	friend class SwapChain;
	friend class VertexBuffer;
	friend class ConstantBuffer;
	friend class IndexBuffer;
	friend class VertexShader;
	friend class PixelShader;
	friend class Texture;



};

