#pragma once
#include "Resource.h"
#include "Prerequisites.h"

struct MaterialSlot
{
	MaterialSlot(): m_material_id(0), m_start_index(0), m_num_indices(0){}
	size_t m_material_id;
	size_t m_start_index;
	size_t m_num_indices;
};

class Mesh:public Resource
{
public:
	Mesh(const wchar_t* full_path);
	virtual ~Mesh();
	const VertexBufferPtr& GetVertexBuffer();
	const IndexBufferPtr& GetIndexBuffer();
	const MaterialSlot& GetMaterialSlot(size_t index);
	size_t GetNumMaterialSlots();
protected:
	VertexBufferPtr mp_vb;
	IndexBufferPtr mp_ib;
	std::vector<MaterialSlot> m_mat_slots;
	friend class DeviceContext;
};

