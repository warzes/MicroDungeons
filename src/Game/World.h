#pragma once

#include "WorldData.h"
#include "WorldChunk.h"

namespace std {
	template<>
	struct hash<Point2>
	{
		std::size_t operator()(const Point2& k) const
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

	TileInfo* GetTile(Point2 tilePosition);
	const TileInfo* GetTile(Point2 tilePosition) const;
	Vector4 GetLight(Vector2 position);
	Point2 Size() const;

	bool TestCollision(const BoundingBox& bbox) const;

private:
	Point2 chunkPosition(Point2 tilePosition);

	WorldData* m_worldData = nullptr;
	std::unordered_map<Point2, WorldChunk> m_chunks;
	Texture m_textureTileset = {};
	Shader m_chunkShader = {};
	int m_uniformFogDensity = 0;
};