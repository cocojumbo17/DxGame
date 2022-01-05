#pragma once
#include "Resource.h"

class Texture:public Resource
{
public:
	Texture(const wchar_t* full_path);
	virtual ~Texture();
protected:
	ID3D11Resource* mp_tex;
	ID3D11ShaderResourceView* mp_shader_resource_view;
	friend class DeviceContext;
};

