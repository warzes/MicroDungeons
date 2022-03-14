#pragma once

#include "Other.h"

class World;

class WorldChunk
{
public:
	WorldChunk(World& world, Texture2D textureDiffuse, Shader chunkShader, Point2 position);
	void Draw();
	void Close();

private:
	World& m_world;
	Point2 m_position;
	Mesh m_mesh;
	Model m_model;
};