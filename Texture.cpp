//The MIT License(MIT)
//
//Copyright(c) 2011 - 2020 Microsoft Corp
//
//Permission is hereby granted, free of charge, to any person obtaining a copy of this
//softwareand associated documentation files(the "Software"), to deal in the Software
//without restriction, including without limitation the rights to use, copy, modify,
//merge, publish, distribute, sublicense, and /or sell copies of the Software, and to
//permit persons to whom the Software is furnished to do so, subject to the following
//conditions :
//
//The above copyright noticeand this permission notice shall be included in all copies
//or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
//CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
//OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#include "Texture.h"
#include <DirectXTex.h>
#include "GraphicsEngine.h"
#include <iostream>

Texture::Texture(const wchar_t* full_path) : Resource(full_path)
{
	DirectX::ScratchImage image_data;
	HRESULT res = DirectX::LoadFromWICFile(full_path, DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, image_data);
	
	if (SUCCEEDED(res))
	{
	
		//STORE THE FILE TEXTURE 
		res = DirectX::CreateTexture(GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->m_d3d_device, image_data.GetImages(),
			image_data.GetImageCount(), image_data.GetMetadata(), (ID3D11Resource**)&m_texture);
		if (FAILED(res))throw std::exception("Texture was not created successfully");

        //CREATE MIPMAP TEXTURE
	    ID3D11Texture2D* tex = nullptr; 
		D3D11_TEXTURE2D_DESC texDesc = {};
		m_texture->GetDesc(&texDesc);
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.CPUAccessFlags = 0;
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texDesc.MipLevels = 8;
		texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		res = GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->m_d3d_device->CreateTexture2D(&texDesc, nullptr, &tex);
		if (FAILED(res))throw std::exception("Texture was not created successfully");

		//COPY THE FILE TEXTURE SUBRESOURCE TO MIPMAP TEXTURE
	    GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->m_imm_context->CopySubresourceRegion(tex,0,0,0,0, m_texture,0,NULL);

		//CREATE SAMPLER STATE AND SHADER RESOURCE VIEW
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = texDesc.Format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = texDesc.MipLevels;
		desc.Texture2D.MostDetailedMip = 0;

		D3D11_SAMPLER_DESC sampler_desc = {};
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = texDesc.MipLevels;

		res = GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->m_d3d_device->CreateSamplerState(&sampler_desc, &m_sampler_state);
		if (FAILED(res))throw std::exception("Texture was not created successfully");

		res = GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->m_d3d_device->CreateShaderResourceView(tex, &desc, &m_shader_res_view);
		if (FAILED(res))throw std::exception("Texture was not created successfully");
	
		//GENERATE THE MIPMAPS 
	    GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->m_imm_context->GenerateMips(m_shader_res_view);

	}
	else
	{
		throw std::exception("Texture was not created successfully");
	}


}

Texture::Texture(const Rect& size, Texture::Type type):Resource(L"")
{
	D3D11_TEXTURE2D_DESC tex_desc = {};
	tex_desc.Width = size.width;
	tex_desc.Height = size.height;

	if (type == Normal)
	{
      tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	  tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE ;
	}
	 
	else if (type == RenderTarget)
	{ 
		tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		tex_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	}
	else if (type == DepthStencil)
	{ 
	  tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	  tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    }


	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.MipLevels = 1;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.MiscFlags = 0;
	tex_desc.ArraySize = 1;
	tex_desc.CPUAccessFlags = 0;

	auto hresult = GraphicsEngine::GetGraphicsEngine()->getRenderSystem()->m_d3d_device->CreateTexture2D(&tex_desc, nullptr, (ID3D11Texture2D**)&m_texture);

	if (FAILED(hresult))
		throw std::exception("Texture was not created successfully");

	if (type == Normal || type == RenderTarget)
	{
		hresult = GraphicsEngine::GetGraphicsEngine()->getRenderSystem()
			  ->m_d3d_device->CreateShaderResourceView(m_texture, NULL, &m_shader_res_view);

		if (FAILED(hresult))
			throw std::exception("Texture was not created successfully");
	}

     if (type == RenderTarget)
	{
		 hresult = GraphicsEngine::GetGraphicsEngine()->getRenderSystem()
			 ->m_d3d_device->CreateRenderTargetView(m_texture, NULL, &m_render_target_view);

		 if (FAILED(hresult))
			 throw std::exception("Texture was not created successfully");
	}
	else if (type == DepthStencil)
	{
		 hresult = GraphicsEngine::GetGraphicsEngine()->getRenderSystem()
			 ->m_d3d_device->CreateDepthStencilView(m_texture, NULL, &m_depth_stencil_view);

		 if (FAILED(hresult))
			 throw std::exception("Texture was not created successfully");
	}


	 m_type = type;
	 m_size = size;
}


Texture::~Texture()
{

    if (m_render_target_view)m_render_target_view->Release();
	if (m_depth_stencil_view)m_depth_stencil_view->Release();
	if (m_sampler_state)m_sampler_state->Release();
	if (m_shader_res_view)m_shader_res_view->Release();
	if (m_texture)m_texture->Release();
}

Rect Texture::getSize()
{
	return m_size;
}

Texture::Type Texture::getType()
{
	return m_type;
}
