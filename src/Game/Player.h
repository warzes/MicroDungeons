#pragma once

class World;
class EnemyManager;

class PlayerCamera
{
	friend class PlayerLogic;
public:
	PlayerCamera();

	static PlayerCamera* Get();

	void Init(const Vector3& startPosition, int rotationCamX, int rotationCamY);

	void Update(const World& world, const EnemyManager& enemyManager, ModelCollisionTest* modelCollision, float deltaTime);

	bool TestCollision(const World& world, const EnemyManager& enemyManager, ModelCollisionTest* modelCollision);
	bool TestCollision(ModelCollisionTest* modelCollision);

	Camera GetCamera() const { return m_camera; }
	BoundingBox GetBoundingBox() const;
	Vector3 GetPosition() const;

	void EnableCursor();
	void DisableCursor();
private:
	void checkInputs();

	Camera m_camera = { };
	Vector2 m_cameraAngle = { 0.0f, 0.0f };
	Vector3 m_position;
	Vector3 m_direction;
	Vector3 m_velocity = { 0, 0, 0 };
	BoundingBox m_collisionBox;
	CollisionModel3D m_colModel;
	float m_speed = 0.125f / 6;
	bool m_cursorEnabled = false;
	bool m_canJump = true;
};

class PlayerLogic
{
public:
	PlayerLogic(PlayerCamera& playerCamera);

private:
	Vector3& m_position;
};