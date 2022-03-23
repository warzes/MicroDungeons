#pragma once

enum class ObjectType
{
	Static,
	Dynamic
};

class RenderObject
{
	friend class RenderObjectManager;
public:
	void Init(ObjectType type, const Model& model);
	void Init(ObjectType type, const std::string& modelFileName, const std::string& diffuseTextureFileName);

	void SetScale(float scale);

	void SetPosition(const Vector3& pos);
	void Move(const Vector3& direction);

	void Update();
	void Draw();

	bool CalculateY(const Vector3& position, float& outY);

	const Vector3& GetPosition() const { return m_position; }
	const BoundingBox& GetBoundingBox() const { return m_bounds; }

private:
	ObjectType m_type;
	Model m_model;
	Texture2D m_diffuseTexture;
	BoundingBox m_bounds;
	Matrix m_matTransform;

	Vector3 m_position = {};
	Vector3 m_direction = {};

	Vector3 m_size = {};
	float m_scale = 1.0f;

	bool m_isMove = true;
};