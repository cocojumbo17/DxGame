#include "pch.h"
#include "TextureManager.h"
#include "Texture.h"
#include "DirectXTex.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"


Texture::Texture(const wchar_t* full_path)
	: Resource(full_path)
{
	DirectX::ScratchImage image_data;
	
	HRESULT hr = DirectX::LoadFromWICFile(full_path, DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, image_data);
	if (SUCCEEDED(hr))
	{
		hr = DirectX::CreateTexture(GraphicsEngine::Instance()->GetRenderSystem()->mp_d3d_device, image_data.GetImages(), image_data.GetImageCount(), image_data.GetMetadata(), &mp_tex);
		if (SUCCEEDED(hr))
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
			desc.Format = image_data.GetMetadata().format;
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipLevels = (UINT)image_data.GetMetadata().mipLevels;
			desc.Texture2D.MostDetailedMip = 0;
			hr = GraphicsEngine::Instance()->GetRenderSystem()->mp_d3d_device->CreateShaderResourceView(mp_tex, &desc, &mp_shader_resource_view);
			if (SUCCEEDED(hr))
				return;
		}
	}
	throw(std::exception("Texture is not created properly."));
}

Texture::~Texture()
{
	SAFE_RELEASE(mp_shader_resource_view);
	SAFE_RELEASE(mp_tex);
}
