#pragma once

class World;

class PlayerCamera
{
public:
	PlayerCamera();

	static PlayerCamera* Get();

	void Init(Vector3 startPosition);

	void Update(World* world, float deltaTime);

	bool TestCollision(World* world);

	Camera GetCamera() { return m_camera; }

	void EnableCursor();
	void DisableCursor();
private:
	void checkInputs();

	Camera m_camera = { 0 };
	Vector3 m_position;
	Vector3 m_direction;
	Vector3 m_velocity = { 0, 0, 0 };
	BoundingBox m_collisionBox;
	float m_speed = 0.125f / 6;

	bool m_canJump = true;
};

