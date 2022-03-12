#pragma once

#include "WorldData.h"
#include "WorldChunk.h"

namespace std {

	template<>
	struct hash<glm::ivec2>
	{
		std::size_t operator()(const glm::ivec2& k) const
		{
			using std::hash;

			// TODO: Revisit this, pretty sure this is a shit hash function.
			return ((hash<int32_t>()(k.x) ^ (hash<int32_t>()(k.y) << 1)) >> 1);
		}
	};

} // namespace std

class World
{
public:
	void Init();
	void Frame();
	void Close();

	TileInfo* GetTile(glm::ivec2 tilePosition);
	glm::vec4 GetLight(glm::vec2 position);

private:
	glm::ivec2 chunkPosition(glm::ivec2 tilePosition);

	WorldData* m_worldData = nullptr;
	std::unordered_map<glm::ivec2, WorldChunk> m_chunks;
};