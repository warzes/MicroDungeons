#pragma once

struct TempResource {};

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	static ResourceManager* Get();

	std::shared_ptr<TempResource> GetTempResource(const std::string& str);
private:
	std::unordered_map<std::string, std::weak_ptr<TempResource>> m_temp;
};