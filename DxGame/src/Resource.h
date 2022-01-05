#pragma once
class Resource
{
public:
	Resource(const wchar_t* full_path);
	virtual ~Resource();
protected:
	const wchar_t* m_full_path;
};

