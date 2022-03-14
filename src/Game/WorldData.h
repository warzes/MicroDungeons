#pragma once

#include "Other.h"

enum class TileType : uint8_t
{
	Air = 0,
	Temp,
	Wall // TODO: rename Solid
};

struct TileInfo
{
	TileType type;
	uint16_t textureWall;
	uint16_t textureFloor;
	uint16_t textureCeiling;
	Vector3 lighting;
};

inline constexpr bool IsTileTypeOpaque(TileType tileType)
{
	return tileType != TileType::Air && tileType != TileType::Temp;
}

inline static constexpr uint64_t ChunkSize = 16;
inline static constexpr uint64_t TilesetSize = 16;

class WorldData
{
public:
	WorldData(const char* file);

	TileInfo* GetTile(Point2 tilePosition);
	const TileInfo* GetTile(Point2 tilePosition) const;
	Vector3 GetTileLight(Point2 tilePosition);

	Point2 Size() const;

	void PerformRadiosity(Point2 tilePosition, Vector3 lightSource);
private:
	Point2 m_size;

	std::vector<TileInfo> m_tileInfo;
};