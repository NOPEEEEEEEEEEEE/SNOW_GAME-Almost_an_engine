#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
#include "Vector3D.h"
#include "Rotator3D.h"
#include <vector>

class DeviceContext
{
public:

	DeviceContext(ID3D11DeviceContext* device_context, RenderSystem* render_system);
	void clearRenderTargetColor(const SwapChainPtr& swap_chain,float red, float green, float blue, float alpha);
	void clearDepthStencil(const SwapChainPtr& swap_chain);

	void clearRenderTargetColor(const TexturePtr& render_target, float red, float green, float blue, float alpha);
	void clearDepthStencil(const TexturePtr& depth_stencil);

	void setRenderTarget(const TexturePtr& render_target , const TexturePtr& depth_stencil);

	//HRESULT createSurface(const TexturePtr& render_target,IDXGISurface* surface);


	void copySubresourceRegion(
		           ID3D11Resource* pDstResource,
		           UINT            DstSubresource,
		           UINT            DstX,
		           UINT            DstY,
		           UINT            DstZ,
		           ID3D11Resource* pSrcResource,
		           UINT            SrcSubresource);


	void setVertexBuffer(const VertexBufferPtr& vertex_buffer);
	void setIndexBuffer(const IndexBufferPtr& index_buffer);

	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location);
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);


	void setViewportSize(UINT width,UINT height);

	void setVertexShader(const VertexShaderPtr& vertex_shader);
	void setPixelShader(const PixelShaderPtr& pixel_shader);

	void setTexture(const VertexShaderPtr& vertex_shader, const TexturePtr* texture, unsigned int num_textures);
	void setTexture(const PixelShaderPtr& pixel_shader, const TexturePtr* texture, unsigned int num_textures);

	void setConstantBuffer(const VertexShaderPtr& vertex_shader, const ConstantBufferPtr& buffer);
	void setConstantBuffer(const PixelShaderPtr& vertex_shader, const ConstantBufferPtr& buffer);

	~DeviceContext();

private:
	ID3D11DeviceContext* m_device_context;
	RenderSystem* m_render_system = nullptr;

    friend class ConstantBuffer;

};

