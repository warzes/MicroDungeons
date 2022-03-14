#include "stdafx.h"
#include "WorldData.h"
//-----------------------------------------------------------------------------
WorldData::WorldData(const char* file)
{
	m_size.x = 32;
	m_size.y = 32;

	for (uint32_t y = 0; y < m_size.y; ++y)
	{
		for (uint32_t x = 0; x < m_size.x; ++x)
		{
			TileInfo tileInfo = {};

			tileInfo.type = TileType::Air;

			if (x == 0 || x == m_size.x - 1)
				tileInfo.type = TileType::Wall;
			else if (y == 0 || y == m_size.y - 1)
				tileInfo.type = TileType::Wall;
			else
			{
				int t = rand() % 100;
				if (t < 4) tileInfo.type = TileType::Wall;
			}

			tileInfo.textureCeiling = 2;
			tileInfo.textureFloor = 0;
			tileInfo.textureWall = 1;
			tileInfo.lighting = { 0, 0, 0 };
			m_tileInfo.push_back(tileInfo);
		}
	}

	for (int i = 0; i < 100; i++)
	{
		float randR = rand() % 255 / 255.0f;
		float randG = rand() % 255 / 255.0f;
		float randB = rand() % 255 / 255.0f;

		int x = rand() % m_size.x;
		int y = rand() % m_size.y;

		PerformRadiosity({ x, y }, { randR, randG, randB });
	}
}
//-----------------------------------------------------------------------------
TileInfo* WorldData::GetTile(glm::ivec2 tilePosition)
{
	if (tilePosition.x < 0 || tilePosition.x >= m_size.x || tilePosition.y < 0 || tilePosition.y >= m_size.y)
		return nullptr;
	return &m_tileInfo[tilePosition.y * m_size.x + tilePosition.x];
}
//-----------------------------------------------------------------------------
const TileInfo* WorldData::GetTile(glm::ivec2 tilePosition) const
{
	if (tilePosition.x < 0 || tilePosition.x >= m_size.x || tilePosition.y < 0 || tilePosition.y >= m_size.y)
		return nullptr;
	return &m_tileInfo[tilePosition.y * m_size.x + tilePosition.x];
}
//-----------------------------------------------------------------------------
Vector3 WorldData::GetTileLight(glm::ivec2 tilePosition)
{
	auto currentTile = GetTile(tilePosition);
	return currentTile ? currentTile->lighting : Vector3{ 0.0f, 0.0f, 0.0f };
}
//-----------------------------------------------------------------------------
glm::ivec2 WorldData::Size() const
{
	return m_size;
}
//-----------------------------------------------------------------------------
void WorldData::PerformRadiosity(glm::ivec2 tilePosition, Vector3 lightSource)
{
	std::queue<std::pair<glm::ivec2, Vector3>> frontier;
	std::set<std::tuple<int32_t, int32_t>> visited;

	frontier.push({ tilePosition, lightSource });

	while (!frontier.empty())
	{
		std::pair<glm::ivec2, Vector3> current = frontier.front();
		frontier.pop();

		glm::ivec2 currentPosition = current.first;
		Vector3 currentLightLevel = current.second;

		if (visited.count({ currentPosition.x, currentPosition.y }))
			continue;
		visited.emplace(currentPosition.x, currentPosition.y);

		TileInfo* currentTile = GetTile(currentPosition);
		if (!currentTile)
			continue;

		if (IsTileTypeOpaque(currentTile->type))
			continue;

		if (currentLightLevel.x == 0.0f && currentLightLevel.y == 0.0f && currentLightLevel.z == 0.0f)
			continue;

		currentTile->lighting.x += currentLightLevel.x;
		currentTile->lighting.y += currentLightLevel.y;
		currentTile->lighting.z += currentLightLevel.z;

		static constexpr float lightLevelStep = 0.25f;
		auto nextLighting = currentLightLevel - Vector3(lightLevelStep, lightLevelStep, lightLevelStep);

		if (nextLighting.x < 0.0f) nextLighting.x = 0.0f;
		if (nextLighting.y < 0.0f) nextLighting.y = 0.0f;
		if (nextLighting.z < 0.0f) nextLighting.z = 0.0f;

		frontier.push({ currentPosition - glm::ivec2(-1, 0), nextLighting });
		frontier.push({ currentPosition - glm::ivec2(1, 0),  nextLighting });
		frontier.push({ currentPosition - glm::ivec2(0, -1), nextLighting });
		frontier.push({ currentPosition - glm::ivec2(0, 1),  nextLighting });
	}
}
//-----------------------------------------------------------------------------