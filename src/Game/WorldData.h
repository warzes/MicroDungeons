#pragma once

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
	glm::vec3 lighting;
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

	TileInfo* GetTile(glm::ivec2 tilePosition);
	const TileInfo* GetTile(glm::ivec2 tilePosition) const;
	glm::vec3 GetTileLight(glm::ivec2 tilePosition);

	glm::ivec2 Size() const;

	void PerformRadiosity(glm::ivec2 tilePosition, glm::vec3 lightSource);
private:
	glm::ivec2 m_size;

	std::vector<TileInfo> m_tileInfo;
};