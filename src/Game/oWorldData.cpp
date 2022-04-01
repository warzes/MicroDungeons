#include "stdafx.h"
#include "oWorldData.h"
//-----------------------------------------------------------------------------
oWorldData::oWorldData(const char* file)
{
	m_size.x = 32;
	m_size.y = 32;

	for (int y = 0; y < m_size.y; ++y)
	{
		for (int x = 0; x < m_size.x; ++x)
		{
			oTileInfo tileInfo = {};

			tileInfo.type = oTileType::Air;

			if (x == 0 || x == m_size.x - 1)
				tileInfo.type = oTileType::Wall;
			else if (y == 0 || y == m_size.y - 1)
				tileInfo.type = oTileType::Wall;
			//else
			//{
			//	int t = rand() % 100;
			//	if (t < 4) tileInfo.type = TileType::Wall;
			//}

			//if (x == 8 && y == 8) tileInfo.type = TileType::Temp;
			//if (x == 8 && y == 9) tileInfo.type = TileType::Temp;
			//if (x == 9 && y == 8) tileInfo.type = TileType::Temp;
			//if (x == 9 && y == 9) tileInfo.type = TileType::Temp;

			tileInfo.textureCeiling = 2;
			tileInfo.textureFloor = 0;
			tileInfo.textureWall = 1;
			tileInfo.lighting = { 0, 0, 0 };
			m_tileInfo.push_back(tileInfo);
		}
	}

	for (int i = 0; i < 100; i++)
	{
		float randR = static_cast<float>(rand() % 255) / 255.0f;
		float randG = static_cast<float>(rand() % 255) / 255.0f;
		float randB = static_cast<float>(rand() % 255) / 255.0f;

		int x = rand() % m_size.x;
		int y = rand() % m_size.y;

		PerformRadiosity({ x, y }, { randR, randG, randB });
	}
}
//-----------------------------------------------------------------------------
oTileInfo* oWorldData::GetTile(Point2 tilePosition)
{
	if (tilePosition.x < 0 || tilePosition.x >= m_size.x || tilePosition.y < 0 || tilePosition.y >= m_size.y)
		return nullptr;
	return &m_tileInfo[static_cast<size_t>(tilePosition.y * m_size.x + tilePosition.x)];
}
//-----------------------------------------------------------------------------
const oTileInfo* oWorldData::GetTile(Point2 tilePosition) const
{
	if (tilePosition.x < 0 || tilePosition.x >= m_size.x || tilePosition.y < 0 || tilePosition.y >= m_size.y)
		return nullptr;
	return &m_tileInfo[static_cast<size_t>(tilePosition.y * m_size.x + tilePosition.x)];
}
//-----------------------------------------------------------------------------
Vector3 oWorldData::GetTileLight(Point2 tilePosition)
{
	auto currentTile = GetTile(tilePosition);
	return currentTile ? currentTile->lighting : Vector3{ 0.0f, 0.0f, 0.0f };
}
//-----------------------------------------------------------------------------
Point2 oWorldData::Size() const
{
	return m_size;
}
//-----------------------------------------------------------------------------
void oWorldData::PerformRadiosity(Point2 tilePosition, Vector3 lightSource)
{
	std::queue<std::pair<Point2, Vector3>> frontier;
	std::set<std::tuple<int32_t, int32_t>> visited;

	frontier.push({ tilePosition, lightSource });

	while (!frontier.empty())
	{
		const auto& current = frontier.front();
		Point2 currentPosition = current.first;
		Vector3 currentLightLevel = current.second;
		frontier.pop();

		if (visited.count({ currentPosition.x, currentPosition.y }))
			continue;
		visited.emplace(currentPosition.x, currentPosition.y);

		oTileInfo* currentTile = GetTile(currentPosition);
		if (!currentTile)
			continue;

		if (oIsTileTypeOpaque(currentTile->type))
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

		frontier.push({ currentPosition - Point2(-1, 0), nextLighting });
		frontier.push({ currentPosition - Point2(1, 0),  nextLighting });
		frontier.push({ currentPosition - Point2(0, -1), nextLighting });
		frontier.push({ currentPosition - Point2(0, 1),  nextLighting });
	}
}
//-----------------------------------------------------------------------------