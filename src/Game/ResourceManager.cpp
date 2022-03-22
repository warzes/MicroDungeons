#include "stdafx.h"
#include "ResourceManager.h"

static ResourceManager* gResourceManager = nullptr;

ResourceManager::ResourceManager()
{
	assert(!gResourceManager);
	gResourceManager = this;
}

ResourceManager::~ResourceManager()
{
	gResourceManager = nullptr;
}

ResourceManager* ResourceManager::Get()
{
	assert(gResourceManager);
	return gResourceManager;
}

std::shared_ptr<TempResource> ResourceManager::GetTempResource(const std::string& str)
{
	std::shared_ptr<TempResource> temp = nullptr;
	auto it = m_temp.find(str);
	if (it != m_temp.end())
	{
		if (auto ptr = it->second.lock()) temp = ptr;
	}

	if (!temp)
	{
		std::cout << str << std::endl;
		temp = std::shared_ptr<TempResource>(new TempResource());
		m_temp[str] = temp;
	}
	return temp;
}