#pragma once
#include "Prerequisites.h"
#include "unordered_map"

class ResourceManager
{
public:
	ResourceManager();
	virtual ~ResourceManager();
	ResourcePtr CreateResourceFromFile(const std::wstring& file_name);
protected:
	virtual Resource* CreateResourceFromConcreteFile(const wchar_t* file_name) = 0;
protected:
	std::unordered_map<std::wstring, ResourcePtr> m_resources;
};

