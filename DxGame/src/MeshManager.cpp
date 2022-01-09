#include "pch.h"
#include "MeshManager.h"
#include <filesystem>
#include "Mesh.h"

MeshManager::MeshManager()
	:ResourceManager()
{
}

MeshManager::~MeshManager()
{
}

MeshPtr MeshManager::CreateMeshFromFile(const std::wstring& file_name)
{
	return std::static_pointer_cast<Mesh>(CreateResourceFromFile(file_name));
}

Resource* MeshManager::CreateResourceFromConcreteFile(const wchar_t* file_name)
{
	Mesh* tex = nullptr;
	try
	{
		tex = new Mesh(file_name);
	}
	catch (const std::exception& e)
	{
		Logger::PrintLog("[ERROR] %s", e.what());
		throw;
	}
	return tex;
}

