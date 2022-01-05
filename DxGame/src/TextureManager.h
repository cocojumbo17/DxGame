#pragma once
#include "ResourceManager.h"

class TextureManager:public ResourceManager
{
public:
	TextureManager();
	virtual ~TextureManager();

	TexturePtr CreateTextureFromFile(const std::wstring& file_name);
protected:
	virtual Resource* CreateResourceFromConcreteFile(const wchar_t* file_name) override;
};

