#pragma once

#include "Other.h"

class oWorld;

class oWorldChunk
{
public:
	oWorldChunk(oWorld& world, Texture2D textureDiffuse, Shader chunkShader, Point2 position);
	void Draw();
	void Close();

private:
	oWorld& m_world;
	Point2 m_position;
	Mesh m_mesh;
	Model m_model;
};