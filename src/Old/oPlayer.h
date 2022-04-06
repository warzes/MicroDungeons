#pragma once

class oWorld;
class EnemyManager;

class oPlayerCamera
{
	friend class PlayerLogic;
public:
	oPlayerCamera();

	static oPlayerCamera* Get();

	void Init(const Vector3& startPosition, int rotationCamX, int rotationCamY);

	void Update(const oWorld& world, const EnemyManager& enemyManager, ModelCollisionTest* modelCollision, float deltaTime);

	bool TestCollision(const oWorld& world, const EnemyManager& enemyManager, ModelCollisionTest* modelCollision);
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
	PlayerLogic(oPlayerCamera& playerCamera);

private:
	Vector3& m_position;
};