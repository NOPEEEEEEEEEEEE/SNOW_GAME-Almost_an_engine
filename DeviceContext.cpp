#include "DeviceContext.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture.h"

#include <exception>


DeviceContext::DeviceContext(ID3D11DeviceContext* device_context,RenderSystem* render_system) : m_render_system(render_system),m_device_context(device_context)
{


}

void DeviceContext::clearRenderTargetColor(const SwapChainPtr& swap_chain, float red, float green, float blue, float alpha)
{
	FLOAT clear_color[] = { red,green,blue,alpha };

	m_device_context->ClearRenderTargetView(swap_chain->m_render_target_view,clear_color);
    m_device_context->ClearDepthStencilView(swap_chain->m_depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,1,0);
	m_device_context->OMSetRenderTargets(1, &swap_chain->m_render_target_view, swap_chain->m_depth_stencil_view);
	
	//m_device_context->ClearRenderTargetView(swap_chain->m_HUD_render_target_view, clear_color);
	//m_device_context->OMSetRenderTargets(2, &swap_chain->m_HUD_render_target_view, NULL);

	
}

void DeviceContext::clearDepthStencil(const SwapChainPtr& swap_chain)
{
	m_device_context->ClearDepthStencilView(swap_chain->m_depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void DeviceContext::clearRenderTargetColor(const TexturePtr& render_target, float red, float green, float blue, float alpha)
{
	if (render_target->m_type != Texture::Type::RenderTarget) return;
	FLOAT clear_color[] = { red,green,blue,alpha };
	m_device_context->ClearRenderTargetView(render_target->m_render_target_view, clear_color);

}

void DeviceContext::clearDepthStencil(const TexturePtr& depth_stencil)
{
	if (depth_stencil->m_type != Texture::Type::DepthStencil) return;
	m_device_context->ClearDepthStencilView(depth_stencil->m_depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void DeviceContext::setRenderTarget(const TexturePtr& render_target, const TexturePtr& depth_stencil)
{
	if (render_target->m_type != Texture::Type::RenderTarget) return;
	if (depth_stencil->m_type != Texture::Type::DepthStencil) return;

	m_device_context->OMSetRenderTargets(1, &render_target->m_render_target_view, depth_stencil->m_depth_stencil_view);

}

void DeviceContext::copySubresourceRegion(ID3D11Resource* pDstResource, UINT DstSubresource, UINT DstX, UINT DstY, UINT DstZ, ID3D11Resource* pSrcResource, UINT SrcSubresource)
{
	m_device_context->CopySubresourceRegion(
		 pDstResource,
		           DstSubresource,
		            DstX,
		            DstY,
		            DstZ,
		           pSrcResource,
		          SrcSubresource,NULL);



}



void DeviceContext::setVertexBuffer(const VertexBufferPtr& vertex_buffer)
{
	UINT stride = vertex_buffer->m_size_vertex;
	UINT offset = 0;

		m_device_context->IASetVertexBuffers(0, 1, &vertex_buffer->m_buffer, &stride, &offset);

		m_device_context->IASetInputLayout(vertex_buffer->m_layout);

}

void DeviceContext::setIndexBuffer(const IndexBufferPtr& index_buffer)
{

	m_device_context->IASetIndexBuffer(index_buffer->m_buffer,DXGI_FORMAT_R32_UINT,0);

}

void DeviceContext::drawTriangleList(UINT vertex_count, UINT start_vertex_index)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->Draw(vertex_count, start_vertex_index);


}

void DeviceContext::drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->DrawIndexed(index_count,start_index_location,start_vertex_index);
	

}

void DeviceContext::drawTriangleStrip(UINT vertex_count, UINT start_vertex_index)
{

	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_device_context->Draw(vertex_count, start_vertex_index);

}

void DeviceContext::setViewportSize(UINT width, UINT height)
{
	D3D11_VIEWPORT viewport{};

	viewport.Width = (FLOAT)width;
	viewport.Height = (FLOAT)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	m_device_context->RSSetViewports(1, &viewport);

}


void DeviceContext::setVertexShader(const VertexShaderPtr& vertex_shader)
{
	m_device_context->VSSetShader(vertex_shader->m_vertex_shader,nullptr,0);

}

void DeviceContext::setPixelShader(const PixelShaderPtr& pixel_shader)
{
	m_device_context->PSSetShader(pixel_shader->m_pixel_shader, nullptr, 0);
}

void DeviceContext::setTexture(const VertexShaderPtr& vertex_shader, const TexturePtr* texture, unsigned int num_textures)
{
	ID3D11ShaderResourceView* res_list[32];
	ID3D11SamplerState* res_sampler[32];
	for (unsigned int i = 0; i < num_textures; i++)
	{
		res_list[i] = texture[i]->m_shader_res_view;
		res_sampler[i] = texture[i]->m_sampler_state;

	}

	m_device_context->VSSetShaderResources(0, num_textures, res_list);
	m_device_context->VSSetSamplers(0,num_textures, res_sampler);

}

void DeviceContext::setTexture(const PixelShaderPtr& pixel_shader, const TexturePtr* texture, unsigned int num_textures)
{
	ID3D11ShaderResourceView* res_list[32];
	ID3D11SamplerState* res_sampler[32];
	for (unsigned int i = 0; i < num_textures; i++)
	{
		res_list[i] = texture[i]->m_shader_res_view;
		res_sampler[i] = texture[i]->m_sampler_state;

	}

	m_device_context->PSSetShaderResources(0, num_textures, res_list);
	m_device_context->PSSetSamplers(0, num_textures, res_sampler);
	
}

void DeviceContext::setConstantBuffer(const VertexShaderPtr& vertex_shader, const ConstantBufferPtr& buffer)
{
	m_device_context->VSSetConstantBuffers(0, 1, &buffer->m_buffer);
}

void DeviceContext::setConstantBuffer(const PixelShaderPtr& pixel_shader, const ConstantBufferPtr& buffer)
{
	m_device_context->PSSetConstantBuffers(0, 1, &buffer->m_buffer);
}

DeviceContext::~DeviceContext()
{
	m_device_context->Release();

}
