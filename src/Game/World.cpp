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

	Image img = GenImageChecked(256, 256, 32, 32, DARKBLUE, WHITE);
	tx = LoadTextureFromImage(img);
	UnloadImage(img);
	SetTextureFilter(tx, TEXTURE_FILTER_POINT);
	SetTextureWrap(tx, TEXTURE_WRAP_CLAMP);

	cube = GenMeshCube(1.0f, 1.0f, 1.0f);
	model2 = LoadModelFromMesh(cube);
	model2.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = tx;
	model2.materials[0].shader = chunkShader;

	shaderFrame = GetShaderLocation(chunkShader, "testa");
}

void World::Frame()
{
	float t = 0.0f;
	SetShaderValue(chunkShader, shaderFrame, &t, SHADER_UNIFORM_FLOAT);
	DrawModel(model2, { 1, 1, 0 }, 1.0f, WHITE);
	DrawCubeTexture(tx, Vector3{ 0, 0.5f, 0 }, 1, 1, 1, WHITE);
}

void World::Close()
{
	UnloadModel(model2);
	UnloadShader(chunkShader);
	UnloadTexture(tx);
}