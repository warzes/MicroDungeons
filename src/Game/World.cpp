#include "stdafx.h"
#include "World.h"

Texture tx;
Shader chunkShader;

Mesh cube;
Model model2;

int shaderFrame;

const char* chunkShaderVs =
"#version 330                       \n"
"in vec3 vertexPosition;            \n"
"in vec2 vertexTexCoord;            \n"
"in vec4 vertexColor;               \n"
"out vec2 fragTexCoord;             \n"
"out vec4 fragColor;                \n"
"uniform mat4 mvp;                  \n"
"void main()                        \n"
"{                                  \n"
"    fragTexCoord = vertexTexCoord; \n"
"    fragColor = vertexColor;       \n"
"    gl_Position = mvp*vec4(vertexPosition, 1.0); \n"
"}                                  \n";
;
const char* chunkShaderFs =
"#version 330       \n"
"in vec2 fragTexCoord;              \n"
"in vec4 fragColor;                 \n"
"uniform sampler2D texture0;        \n"
"uniform vec4 colDiffuse;           \n"
"uniform float testa;           \n"
"out vec4 finalColor;               \n"
"void main()                        \n"
"{                                  \n"
"    vec4 texelColor = texture(texture0, fragTexCoord)*colDiffuse*fragColor;   \n"
"    float gray = dot(texelColor.rgb, vec3(0.299, 0.587, 0.114));        \n"
"    finalColor = vec4(gray, gray, testa, texelColor.a);        \n"
"}                                  \n";
;

void World::Init()
{
	chunkShader = LoadShaderFromMemory(chunkShaderVs, chunkShaderFs);

	//Image img = GenImageChecked(256, 256, 32, 32, DARKBLUE, WHITE);
	//tx = LoadTextureFromImage(img);
	tx = LoadTexture("tileset_textures.png");
	//UnloadImage(img);
	SetTextureFilter(tx, TEXTURE_FILTER_POINT);
	SetTextureWrap(tx, TEXTURE_WRAP_MIRROR_REPEAT);

	cube = GenMeshCube(1.0f, 1.0f, 1.0f);
	model2 = LoadModelFromMesh(cube);
	//model2 = LoadModelFromMesh(GenMeshPlane2(1.0f, 1.0f, 1, 1));

	model2.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = tx;
	model2.materials[0].shader = chunkShader;

	shaderFrame = GetShaderLocation(chunkShader, "testa");


	m_worldData = new WorldData("");

	for (int i = 0; i < 100; i++)
	{
		float randR = rand() % 255 / 255.0f;
		float randG = rand() % 255 / 255.0f;
		float randB = rand() % 255 / 255.0f;

		int x = rand() % m_worldData->Size().x;
		int y = rand() % m_worldData->Size().y;

		m_worldData->PerformRadiosity({ x, y }, { randR, randG, randB });
	}

	// Generate level chunks.
	for (int32_t z = 0; z < m_worldData->Size().y; z += ChunkSize)
	{
		for (int32_t x = 0; x < m_worldData->Size().x; x += ChunkSize)
		{
			m_chunks.insert({ chunkPosition({x, z}), WorldChunk{*this, tx, {x, z}} });
		}
	}
}

void World::Frame()
{
	float t = 0.0f;
	SetShaderValue(chunkShader, shaderFrame, &t, SHADER_UNIFORM_FLOAT);
	DrawModel(model2, { 1, 0.0001, 0 }, 1.0f, WHITE);
	DrawCubeTexture(tx, Vector3{ 0, 0.5f, 0 }, 1, 1, 1, WHITE);

	for (auto it = m_chunks.begin(); it != m_chunks.end(); ++it)
	{
		it->second.Draw();
	}
}

void World::Close()
{
	for (auto it = m_chunks.begin(); it != m_chunks.end(); ++it)
	{
		it->second.Close();
	}
	m_chunks.clear();

	UnloadModel(model2);
	UnloadShader(chunkShader);
	UnloadTexture(tx);
	delete m_worldData;
}

TileInfo* World::GetTile(glm::ivec2 tile_position)
{
	return m_worldData->GetTile(tile_position);
}

glm::vec4 World::GetLight(glm::vec2 position)
{
	glm::vec3 light_current = m_worldData->GetTileLight(position);

	int64_t x = (int64_t)position.x;
	int64_t y = (int64_t)position.y;

	auto light_north = m_worldData->GetTileLight({ x, y - 1 });
	auto light_northwest = m_worldData->GetTileLight({ x - 1, y - 1 });
	auto light_west = m_worldData->GetTileLight({ x - 1, y });
	auto light_southwest = m_worldData->GetTileLight({ x - 1, y + 1 });
	auto light_south = m_worldData->GetTileLight({ x, y + 1 });
	auto light_southeast = m_worldData->GetTileLight({ x + 1, y + 1 });
	auto light_east = m_worldData->GetTileLight({ x + 1, y });
	auto light_northeast = m_worldData->GetTileLight({ x + 1, y - 1 });

	auto northwest = (light_north + light_west + light_northwest + light_current) / 4.0f;
	auto northeast = (light_north + light_east + light_northeast + light_current) / 4.0f;
	auto southwest = (light_west + light_southwest + light_south + light_current) / 4.0f;
	auto southeast = (light_south + light_southeast + light_east + light_current) / 4.0f;

	glm::vec2 interpolation_coefficient = position - glm::vec2(glm::ivec2(position));

	auto north_interpolated = glm::mix(northwest, northeast, interpolation_coefficient.x);
	auto south_interpolated = glm::mix(southwest, southeast, interpolation_coefficient.x);
	auto final_interpolated = glm::mix(north_interpolated, south_interpolated, interpolation_coefficient.y);

	static constexpr glm::vec4 ambient_color_k{ 0.25f, 0.25f, 0.25f, 1.0f };
	return ambient_color_k + glm::vec4{ final_interpolated, 1.0f } *(glm::vec4(1, 1, 1, 1) - ambient_color_k);
}

glm::ivec2 World::chunkPosition(glm::ivec2 tile_position)
{
	glm::ivec2 chunk_position;

	for (auto i = 0; i < 2; ++i)
	{
		if (tile_position[i] >= 0)
			chunk_position[i] = tile_position[i] / ChunkSize;
		else
			chunk_position[i] = ((tile_position[i] + 1) / ChunkSize) - 1;
	}

	return chunk_position;
}