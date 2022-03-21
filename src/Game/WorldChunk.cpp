#include "stdafx.h"
#include "WorldChunk.h"
#include "World.h"
//-----------------------------------------------------------------------------
struct VertexWorldChunk
{
	VertexWorldChunk(Vector3 position, Vector2 uv, Vector4 color) : position(position), uv(uv), color(color) {}

	Vector3 position;
	Vector2 uv;
	Vector4 color;
};
//-----------------------------------------------------------------------------
enum class TileSide
{
	Top,
	Bottom,
	Forward,
	Back,
	Left,
	Right
};
//-----------------------------------------------------------------------------
enum class TileVertexId
{
	BottomNorthwest,
	BottomNortheast,
	BottomSouthwest,
	BottomSoutheast,

	TopNorthwest,
	TopNortheast,
	TopSouthwest,
	TopSoutheast
};
//-----------------------------------------------------------------------------
// Tile vertex positions which then make up sides. References TileVertexId.
inline static constexpr Vector3 TileVertexPositions[] = 
{
	Vector3{-0.5f, 0.0f, -0.5f},
	Vector3{0.5f, 0.0f, -0.5f},
	Vector3{-0.5f, 0.0f, 0.5f},
	Vector3{0.5f, 0.0f, 0.5f},

	Vector3{-0.5f, 1.0f, -0.5f},
	Vector3{0.5f, 1.0f, -0.5f},
	Vector3{-0.5f, 1.0f, 0.5f},
	Vector3{0.5f, 1.0f, 0.5f},
};
//-----------------------------------------------------------------------------
// Tile texture coordinates. Order is always 0->1->2->3.
inline static constexpr Vector2 TileVertexUVs[] =
{
	Vector2{0.0f, 0.0f},
	Vector2{1.0f, 0.0f},
	Vector2{0.0f, 1.0f},
	Vector2{1.0f, 1.0f},
};
//-----------------------------------------------------------------------------
// Indices reference TileSide.
inline static constexpr std::array<std::array<int32_t, 6>, 6> SideVertexIndices = 
{
	std::array<int32_t, 6>{6, 4, 7, 4, 5, 7},
	std::array<int32_t, 6>{0, 2, 1, 2, 3, 1},
	std::array<int32_t, 6>{6, 2, 7, 2, 3, 7},
	std::array<int32_t, 6>{5, 1, 4, 1, 0, 4},
	std::array<int32_t, 6>{4, 0, 6, 0, 2, 6},
	std::array<int32_t, 6>{7, 3, 5, 3, 1, 5},
};
//-----------------------------------------------------------------------------
inline static constexpr std::array<int32_t, 6> VertexUVIndices = { 0, 2, 1, 2, 3, 1 };
//-----------------------------------------------------------------------------
WorldChunk::WorldChunk(World& world, Texture2D textureDiffuse, Shader chunkShader, Point2 position)
	: m_world(world)
	, m_position(position)
{
	std::vector<VertexWorldChunk> vertexData;
	for (auto y = m_position.y; y < m_position.y + ChunkSize; ++y)
	{
		for (auto x = m_position.x; x < m_position.x + ChunkSize; ++x)
		{
			TileInfo* tile = m_world.GetTile({ x, y });
			Vector3 offset{ (float)x, 0.0f, (float)y };

			auto createVerticesFunc = [&](TileSide side) 
			{
				auto vertexIndices = SideVertexIndices[static_cast<size_t>(side)];

				uint16_t tileTexture = tile->textureWall;
				if (side == TileSide::Top)
					tileTexture = tile->textureCeiling;
				else if (side == TileSide::Bottom)
					tileTexture = tile->textureFloor;

				for (size_t i = 0; i < vertexIndices.size(); ++i)
				{
					const auto vertexIndex = vertexIndices[i];
					const auto position = TileVertexPositions[vertexIndex] + offset;

					static constexpr Vector2 uvSize
					{
						1.0f / TilesetSize,
						1.0f / TilesetSize,
					};
					const Vector2 tileUV
					{ 
						static_cast<float>(tileTexture % TilesetSize) / TilesetSize,
						static_cast<float>(tileTexture / TilesetSize) / TilesetSize 
					};
					auto tileUVFactors = TileVertexUVs[VertexUVIndices[i]];

					Vector2 uv
					{
						tileUV.x + tileUVFactors.x * uvSize.x,
						tileUV.y + tileUVFactors.y * uvSize.y,
					};

					Vector4 color{};

					switch (static_cast<TileVertexId>(vertexIndex))
					{
					case TileVertexId::BottomNorthwest:
					case TileVertexId::TopNorthwest: color = m_world.GetLight(Vector2( (float)x,        (float)y )); break;
					case TileVertexId::BottomNortheast:
					case TileVertexId::TopNortheast: color = m_world.GetLight(Vector2( (float)x + 1.0f, (float)y )); break;
					case TileVertexId::BottomSouthwest:
					case TileVertexId::TopSouthwest: color = m_world.GetLight(Vector2( (float)x,        (float)y + 1.0f)); break;
					case TileVertexId::BottomSoutheast:
					case TileVertexId::TopSoutheast: color = m_world.GetLight(Vector2( (float)x + 1.0f, (float)y + 1.0f)); break;
					}

					vertexData.emplace_back(position, uv, color);
				}
			};

			if (tile->type == TileType::Air)
			{
				createVerticesFunc(TileSide::Bottom);
				createVerticesFunc(TileSide::Top);
			}
			else if (tile->type == TileType::Temp)
			{
				createVerticesFunc(TileSide::Bottom);
			}
			else if (tile->type == TileType::Wall)
			{
				auto tileSouth = m_world.GetTile({ x, y + 1 });
				auto tileNorth = m_world.GetTile({ x, y - 1 });
				auto tileWest = m_world.GetTile({ x - 1, y });
				auto tileEast = m_world.GetTile({ x + 1, y });

				if (tileSouth && !IsTileTypeOpaque(tileSouth->type))
					createVerticesFunc(TileSide::Forward);

				if (tileNorth && !IsTileTypeOpaque(tileNorth->type))
					createVerticesFunc(TileSide::Back);

				if (tileWest && !IsTileTypeOpaque(tileWest->type))
					createVerticesFunc(TileSide::Left);

				if (tileEast && !IsTileTypeOpaque(tileEast->type))
					createVerticesFunc(TileSide::Right);
			}
		}
	}

	// генерация вершин
	size_t vertexCount = vertexData.size();

	m_mesh = { 0 };
	m_mesh.vertexCount = static_cast<int>(vertexCount);
	m_mesh.vertices = (float*)RL_MALLOC(m_mesh.vertexCount * 3 * sizeof(float));
	m_mesh.texcoords = (float*)RL_MALLOC(m_mesh.vertexCount * 2 * sizeof(float));
	m_mesh.normals = (float*)RL_MALLOC(m_mesh.vertexCount * 3 * sizeof(float));
	m_mesh.colors = (unsigned char*)RL_MALLOC(m_mesh.vertexCount * 4 * sizeof(unsigned char));
	//mesh.indices = (unsigned short*)RL_MALLOC(mesh.triangleCount * 3 * sizeof(unsigned short));

	for (size_t i = 0; i < vertexCount; i++)
	{
		m_mesh.vertices[3 * i + 0] = vertexData[i].position.x;
		m_mesh.vertices[3 * i + 1] = vertexData[i].position.y;
		m_mesh.vertices[3 * i + 2] = vertexData[i].position.z;

		m_mesh.texcoords[2 * i + 0] = vertexData[i].uv.x;
		m_mesh.texcoords[2 * i + 1] = vertexData[i].uv.y;

		// Vector3.up;
		m_mesh.normals[3 * i + 0] = 0.0f;
		m_mesh.normals[3 * i + 1] = 1.0f;
		m_mesh.normals[3 * i + 2] = 0.0f;

		m_mesh.colors[4 * i + 0] = (unsigned char)(vertexData[i].color.x * 255.0f);
		m_mesh.colors[4 * i + 1] = (unsigned char)(vertexData[i].color.y * 255.0f);
		m_mesh.colors[4 * i + 2] = (unsigned char)(vertexData[i].color.z * 255.0f);
		m_mesh.colors[4 * i + 3] = 255;
	}

	// Upload vertex data to GPU (static mesh)
	UploadMesh(&m_mesh, false);

	m_model = LoadModelFromMesh(m_mesh);
	m_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = textureDiffuse;
	m_model.materials[0].shader = chunkShader;
}
//-----------------------------------------------------------------------------
void WorldChunk::Draw()
{
	rlDisableBackfaceCulling();
	DrawModel(m_model, { 0.0f, 0.0f, 0.0f }, 1.0f, WHITE); // позиции уже вшиты в чанки
	//rlEnableBackfaceCulling();
}
//-----------------------------------------------------------------------------
void WorldChunk::Close()
{
	UnloadModel(m_model);
}
//-----------------------------------------------------------------------------