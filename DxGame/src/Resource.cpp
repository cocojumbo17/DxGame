#include "pch.h"
#include "ResourceManager.h"
#include "Resource.h"

Resource::Resource(const wchar_t* full_path)
	: m_full_path(full_path)
{
}

Resource::~Resource()
{
}
