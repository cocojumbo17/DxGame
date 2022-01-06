#pragma once
#include "ResourceManager.h"

class MeshManager:public ResourceManager
{
public:
	MeshManager();
	virtual ~MeshManager();

	MeshPtr CreateMeshFromFile(const std::wstring& file_name);
protected:
	virtual Resource* CreateResourceFromConcreteFile(const wchar_t* file_name) override;
};

