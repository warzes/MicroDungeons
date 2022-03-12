#pragma once

class World;

class WorldChunk
{
public:
	WorldChunk(World& world, Texture2D textureDiffuse, Shader chunkShader, glm::ivec2 position);
	void Draw();
	void Close();

private:
	World& m_world;
	glm::ivec2 m_position;
	Mesh m_mesh;
	Model m_model;
};