#include "pch.h"
#include "TextureManager.h"
#include <filesystem>
#include "Texture.h"

TextureManager::TextureManager()
	:ResourceManager()
{
}

TextureManager::~TextureManager()
{
}

TexturePtr TextureManager::CreateTextureFromFile(const std::wstring& file_name)
{
	return std::static_pointer_cast<Texture>(CreateResourceFromFile(file_name));
}

Resource* TextureManager::CreateResourceFromConcreteFile(const wchar_t* file_name)
{
	Texture* tex = nullptr;
	try
	{
		tex = new Texture(file_name);
	}
	catch (const std::exception&)
	{
		throw;
	}
	return tex;
}

