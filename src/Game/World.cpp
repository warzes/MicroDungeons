#include "stdafx.h"
#include "World.h"

Texture tx;
Shader chunkShader;

Mesh cube;
Model model2;

int uniformFogDensity;

constexpr const char* chunkShaderVs = R"(
#version 330
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec4 vertexColor;
out vec2 fragTexCoord;
out vec4 fragColor;
out vec4 viewSpacePosition;
uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProjection;
void main()
{
	fragTexCoord = vertexTexCoord;
	fragColor = vertexColor;
	mat4 modelView = matView * matModel;
	vec4 eyePosition = modelView * vec4(vertexPosition, 1.0);
	gl_Position = matProjection * eyePosition;
	viewSpacePosition = eyePosition;
}
)";

// оттенки серого
//const char* chunkShaderFs =
//"#version 330       \n"
//"in vec2 fragTexCoord;              \n"
//"in vec4 fragColor;                 \n"
//"uniform sampler2D texture0;        \n"
//"uniform vec4 colDiffuse;           \n"
//"uniform float testa;               \n"
//"out vec4 finalColor;               \n"
//"void main()                        \n"
//"{                                  \n"
//"    vec4 texelColor = texture(texture0, fragTexCoord);           \n"
//"    if (texelColor.a <= 0.1)                                     \n"
//"        discard;                                                 \n"
//"    texelColor = texelColor*colDiffuse*fragColor;                \n"
//"    float gray = dot(texelColor.rgb, vec3(0.299, 0.587, 0.114)); \n"
//"    finalColor = vec4(gray, gray, testa, texelColor.a);        \n"
//"}                                  \n";
//;
constexpr const char* chunkShaderFs = R"(
#version 330
in vec2 fragTexCoord;
in vec4 fragColor;
in vec4 viewSpacePosition;
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float uniformFogDensity;
out vec4 finalColor;
void main()
{
	vec4 texelColor = texture(texture0, fragTexCoord);
	if (texelColor.a <= 0.1)
		discard;
	finalColor = texelColor*colDiffuse*fragColor;
	// fog
	float distance = length(viewSpacePosition);
	float fogFactor = 1.0 /exp(distance * uniformFogDensity * distance * uniformFogDensity);
	fogFactor = clamp( fogFactor, 0.0, 1.0 );
	finalColor.rgb *= fogFactor;
}
)";


void World::Init()
{
	chunkShader = LoadShaderFromMemory(chunkShaderVs, chunkShaderFs);

	tx = LoadTexture("tileset_textures.png");
	SetTextureFilter(tx, TEXTURE_FILTER_POINT);
	SetTextureWrap(tx, TEXTURE_WRAP_MIRROR_REPEAT);

	cube = GenMeshCube(1.0f, 1.0f, 1.0f);
	model2 = LoadModelFromMesh(cube);
	model2.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = tx;
	model2.materials[0].shader = chunkShader;

	//shaderFrame = GetShaderLocation(chunkShader, "testa");
	uniformFogDensity = GetShaderLocation(chunkShader, "uniformFogDensity");

	m_worldData = new WorldData("");

	// Generate level chunks.
	for (int32_t z = 0; z < m_worldData->Size().y; z += ChunkSize)
	{
		for (int32_t x = 0; x < m_worldData->Size().x; x += ChunkSize)
		{
			m_chunks.insert({ chunkPosition({x, z}), WorldChunk{*this, tx, chunkShader, {x, z}} });
		}
	}
}

void World::Frame()
{
	//float FogDensity = 0.10f;
	float FogDensity = 0.50f;
	SetShaderValue(chunkShader, uniformFogDensity, &FogDensity, SHADER_UNIFORM_FLOAT);
	//DrawModel(model2, { 1, 0.0001, 0 }, 1.0f, WHITE);
	//DrawCubeTexture(tx, Vector3{ 0, 0.5f, 0 }, 1, 1, 1, WHITE);

	//DrawModel(model2, { 2.1, 0.5, 2.1 }, 1.0f, WHITE);

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

TileInfo* World::GetTile(glm::ivec2 tilePosition)
{
	return m_worldData->GetTile(tilePosition);
}

const TileInfo* World::GetTile(glm::ivec2 tilePosition) const
{
	return m_worldData->GetTile(tilePosition);
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

glm::ivec2 World::Size() const
{
	return m_worldData->Size();
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