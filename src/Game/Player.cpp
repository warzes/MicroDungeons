#include "stdafx.h"
#include "Player.h"
#include "World.h"
//-----------------------------------------------------------------------------
static PlayerCamera* gPlayer = nullptr;
#define MOUSE_SENSITIVITY 0.003f
//-----------------------------------------------------------------------------
PlayerCamera::PlayerCamera()
{
	gPlayer = this;

	m_camera.up = { 0.0f, 1.0f, 0.0f };
	m_camera.fovy = 70.0f;
	m_camera.projection = CAMERA_PERSPECTIVE;

	m_collisionBox.min = { 0.0f, 0.0f, 0.0f };
	m_collisionBox.max = { 0.4f, 0.6f, 0.4f };
}
//-----------------------------------------------------------------------------
PlayerCamera* PlayerCamera::Get()
{
	return gPlayer;
}
//-----------------------------------------------------------------------------
void PlayerCamera::Init(const Vector3& startPosition, int rotationCamX, int rotationCamY)
{
	SetCameraMode(m_camera, CAMERA_CUSTOM);
	m_position = { startPosition.x, startPosition.y, startPosition.z };
	this->DisableCursor();
	m_cameraAngle.x = (float)rotationCamX * DEG2RAD;
	m_cameraAngle.y = (float)rotationCamY * DEG2RAD;
}
//-----------------------------------------------------------------------------
void PlayerCamera::Update(const World& world, float deltaTime)
{
	//Gravity
	m_velocity.y -= 0.012f * (deltaTime * 60.0f);

	//Calculate velocity with delta time
	Vector3 velXdt = Vector3Scale(m_velocity, deltaTime * 60.0f);

	m_position.x += velXdt.x;
	if (TestCollision(world)) m_position.x -= velXdt.x;
	m_position.z += velXdt.z;
	if (TestCollision(world)) m_position.z -= velXdt.z;

	m_position.y += velXdt.y;
	if (TestCollision(world) || m_position.y < 0.0f)
	{
		m_position.y -= velXdt.y;
		if (m_velocity.y <= 0.0f) m_canJump = true;
		m_velocity.y = 0.0f;
	}

	m_camera.position = m_position;
	m_camera.position.y += 0.5f;

	m_velocity.x -= velXdt.x / 6.0f;
	m_velocity.z -= velXdt.z / 6.0f;

	checkInputs();
}
//-----------------------------------------------------------------------------
bool PlayerCamera::TestCollision(const World& world)
{
	BoundingBox pB = m_collisionBox;
	pB.min = Vector3Add(pB.min, m_position);
	pB.max = Vector3Add(pB.max, m_position);

	// TODO:

	return false;
}
//-----------------------------------------------------------------------------
void PlayerCamera::EnableCursor()
{
	m_cursorEnabled = true;
	::EnableCursor();
}
//-----------------------------------------------------------------------------
void PlayerCamera::DisableCursor()
{
	m_cursorEnabled = false;
	::DisableCursor();
	SetMousePosition(GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f);
}
//-----------------------------------------------------------------------------
void PlayerCamera::checkInputs()
{
	Vector2 mousePositionDelta = GetMouseDelta();

	if (!m_cursorEnabled)
	{
		m_cameraAngle.x -= (mousePositionDelta.x * -MOUSE_SENSITIVITY);
		m_cameraAngle.y -= (mousePositionDelta.y * -MOUSE_SENSITIVITY);

		const float maxCamAngleY = PI - 0.01f;
		const float minCamAngleY = 0.01f;

		if (m_cameraAngle.y >= maxCamAngleY)
			m_cameraAngle.y = maxCamAngleY;
		else if (m_cameraAngle.y <= minCamAngleY)
			m_cameraAngle.y = minCamAngleY;
	}

	//Calculate direction vectors of the camera angle
	const float cx = cosf(m_cameraAngle.x);
	const float sx = sinf(m_cameraAngle.x);

	const float cx90 = cosf(m_cameraAngle.x + PI * 0.5f);
	const float sx90 = sinf(m_cameraAngle.x + PI * 0.5f);

	const float sy = sinf(m_cameraAngle.y);
	const float cy = cosf(m_cameraAngle.y);

	const float forwardX = cx * sy;
	const float forwardY = cy;
	const float forwardZ = sx * sy;

	if (!m_cursorEnabled)
	{
		if (IsKeyDown(KEY_SPACE) && m_canJump)
		{
			m_velocity.y += 0.2f;
			m_canJump = false;
		}

		if (IsKeyDown(KEY_W))
		{
			m_velocity.z += sx * m_speed;
			m_velocity.x += cx * m_speed;
		}

		if (IsKeyDown(KEY_S))
		{
			m_velocity.z -= sx * m_speed;
			m_velocity.x -= cx * m_speed;
		}

		if (IsKeyDown(KEY_A))
		{
			m_velocity.z -= sx90 * m_speed;
			m_velocity.x -= cx90 * m_speed;
		}

		if (IsKeyDown(KEY_D))
		{
			m_velocity.z += sx90 * m_speed;
			m_velocity.x += cx90 * m_speed;
		}
	}
	// Place camera's target to the direction looking at.
	m_camera.target.x = m_camera.position.x + forwardX;
	m_camera.target.y = m_camera.position.y + forwardY;
	m_camera.target.z = m_camera.position.z + forwardZ;

	UpdateCamera(&m_camera);
}
//-----------------------------------------------------------------------------