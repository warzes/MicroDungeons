#pragma once

class RenderObject
{
	friend class RenderObjectManager;
public:
	void Init(const Model& model);
	void Init(const std::string& modelFileName, const std::string& diffuseTextureFileName);

	void SetScale(float scale);

	void SetPosition(const Vector3& pos);
	void Move(const Vector3& direction);

	void Update();
	void Draw();

	bool CalculateY(const Vector3& position, float& outY);

	const Vector3& GetPosition() const { return m_position; }
	const BoundingBox& GetBoundingBox() const { return m_bounds; }

private:
	Model m_model;
	Texture2D m_diffuseTexture;
	BoundingBox m_bounds;
	Matrix m_matTransform;

	Vector3 m_position = {};
	Vector3 m_direction = {};

	float m_scale = 1.0f;
};