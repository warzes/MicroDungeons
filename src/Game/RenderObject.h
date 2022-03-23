#pragma once

class RenderObject
{
public:
	void Init(const std::string& modelFileName, const std::string& diffuseTextureFileName);

	void SetScale(float scale);

	void SetPosition(const Vector3& pos);
	void Move(const Vector3& direction);

	void Update();
	void Draw();

	bool Collisions(const Vector3& position, Vector3& outPoint);

private:
	Model m_model;
	Texture2D m_diffuseTexture;
	BoundingBox m_bounds;

	Vector3 m_position = {};
	Vector3 m_direction = {};

	float m_scale = 1.0f;
};