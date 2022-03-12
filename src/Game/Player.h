#pragma once

class World;

class PlayerCamera
{
public:
	PlayerCamera();

	static PlayerCamera* Get();

	void Init(const Vector3& startPosition, int rotationCamX, int rotationCamY);

	void Update(const World& world, float deltaTime);

	bool TestCollision(const World& world);

	Camera GetCamera() const { return m_camera; }

	void EnableCursor();
	void DisableCursor();
private:
	void checkInputs();

	Camera m_camera = { 0 };
	Vector2 m_cameraAngle = { 0.0f, 0.0f };
	Vector3 m_position;
	Vector3 m_direction;
	Vector3 m_velocity = { 0, 0, 0 };
	BoundingBox m_collisionBox;
	float m_speed = 0.125f / 6;
	bool m_cursorEnabled = false;
	bool m_canJump = true;
};