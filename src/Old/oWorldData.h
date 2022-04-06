#pragma once

#include "Other.h"

enum class oTileType : uint8_t
{
	Air = 0,
	Temp,
	Wall // TODO: rename Solid
};

struct oTileInfo
{
	oTileType type;
	uint16_t textureWall;
	uint16_t textureFloor;
	uint16_t textureCeiling;
	Vector3 lighting;
};

inline constexpr bool oIsTileTypeOpaque(oTileType tileType)
{
	return tileType != oTileType::Air && tileType != oTileType::Temp;
}

inline static constexpr int oChunkSize = 16;
inline static constexpr uint64_t oTilesetSize = 16;

class oWorldData
{
public:
	oWorldData(const char* file);

	oTileInfo* GetTile(Point2 tilePosition);
	const oTileInfo* GetTile(Point2 tilePosition) const;
	Vector3 GetTileLight(Point2 tilePosition);

	Point2 Size() const;

	void PerformRadiosity(Point2 tilePosition, Vector3 lightSource);
private:
	Point2 m_size;

	std::vector<oTileInfo> m_tileInfo;
};