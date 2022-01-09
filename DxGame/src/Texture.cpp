#include "pch.h"
#include "TextureManager.h"
#include "Texture.h"
#include "DirectXTex.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include <locale>
#include <codecvt>


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
			if (FAILED(hr))
			{
				std::stringstream ss;
				std::string filename = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(full_path);
				ss << "Texture for file " << filename << " is not created properly.";
				throw(std::exception(ss.str().c_str()));
			}
			D3D11_SAMPLER_DESC desc2 = { 0 };
			desc2.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			desc2.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			desc2.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			desc2.Filter = D3D11_FILTER_ANISOTROPIC;
			desc2.MinLOD = 0;
			desc2.MaxLOD = (UINT)image_data.GetMetadata().mipLevels;
			hr = GraphicsEngine::Instance()->GetRenderSystem()->mp_d3d_device->CreateSamplerState(&desc2, &mp_sampler_state);
			if (FAILED(hr))
			{
				std::stringstream ss;
				std::string filename = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(full_path);
				ss << "Texture for file " << filename << " is not created properly because of incorrect creation of Sampler.";
				throw(std::exception(ss.str().c_str()));
			}
		}
	}
}

Texture::~Texture()
{
	SAFE_RELEASE(mp_sampler_state);
	SAFE_RELEASE(mp_shader_resource_view);
	SAFE_RELEASE(mp_tex);
}
