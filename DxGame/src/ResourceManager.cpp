#include "pch.h"
#include "ResourceManager.h"
#include "Resource.h"
#include <filesystem>

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

ResourcePtr ResourceManager::CreateResourceFromFile(const std::wstring& file_name)
{
	std::wstring full_path = std::filesystem::absolute(file_name).c_str();
	auto it = m_resources.find(full_path);
	if (it != m_resources.end())
		return it->second;

	Resource* resource = CreateResourceFromConcreteFile(full_path.c_str());
	if (resource)
	{
		ResourcePtr res(resource);
		m_resources[full_path] = res;
		return res;
	}
	return nullptr;
}
