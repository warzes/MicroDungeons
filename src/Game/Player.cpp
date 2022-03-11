#include "stdafx.h"
#include "Player.h"
//-----------------------------------------------------------------------------
static PlayerCamera* gPlayer = nullptr;
#define MOUSE_SENSITIVITY 0.003f
static Vector2 oldMousePos = { 0.0f, 0.0f };
static Vector2 cameraAngle = { 0.0f, 0.0f };
static bool cursorEnabled = false; // TODO:
//-----------------------------------------------------------------------------
PlayerCamera::PlayerCamera()
{
	gPlayer = this;

	m_camera.up = { 0.0f, 1.0f, 0.0f };
	m_camera.fovy = 70.0f;
	m_camera.projection = CAMERA_PERSPECTIVE;

	m_collisionBox.min = { 0, 0, 0 };
	m_collisionBox.max = { 0.4f, 0.6f, 0.4f };
}
//-----------------------------------------------------------------------------
PlayerCamera* PlayerCamera::Get()
{
	return gPlayer;
}
//-----------------------------------------------------------------------------
void PlayerCamera::Init(Vector3 startPosition)
{
	SetCameraMode(m_camera, CAMERA_CUSTOM);
	m_position = { startPosition.x, startPosition.y, startPosition.z };
	DisableCursor();
}
//-----------------------------------------------------------------------------
void PlayerCamera::Update(World* world, float deltaTime)
{
	//Gravity
	m_velocity.y -= 0.012f * (deltaTime * 60);

	//Calculate velocity with delta time
	Vector3 velXdt = Vector3Scale(m_velocity, deltaTime * 60);

	m_position.x += velXdt.x;
	if (TestCollision(world)) m_position.x -= velXdt.x;
	m_position.z += velXdt.z;
	if (TestCollision(world)) m_position.z -= velXdt.z;

	m_position.y += velXdt.y;
	if (TestCollision(world) || m_position.y < 0)
	{
		m_position.y -= velXdt.y;
		if (m_velocity.y <= 0) m_canJump = true;
		m_velocity.y = 0;
	}

	m_camera.position = m_position;
	m_camera.position.y += 0.5f;

	m_velocity.x -= velXdt.x / 6.0f;
	m_velocity.z -= velXdt.z / 6.0f;

	checkInputs();
}
//-----------------------------------------------------------------------------
bool PlayerCamera::TestCollision(World* world)
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
	::EnableCursor();
}
//-----------------------------------------------------------------------------
void PlayerCamera::DisableCursor()
{
	::DisableCursor();
}
//-----------------------------------------------------------------------------
void PlayerCamera::checkInputs()
{
	Vector2 mousePositionDelta = { 0.0f, 0.0f };
	Vector2 mousePos = GetMousePosition();
	mousePositionDelta.x = mousePos.x - oldMousePos.x;
	mousePositionDelta.y = mousePos.y - oldMousePos.y;

	oldMousePos = GetMousePosition();

	if (!cursorEnabled)
	{
		cameraAngle.x -= (mousePositionDelta.x * -MOUSE_SENSITIVITY);
		cameraAngle.y -= (mousePositionDelta.y * -MOUSE_SENSITIVITY);

		const float maxCamAngleY = PI - 0.01f;
		const float minCamAngleY = 0.01f;

		if (cameraAngle.y >= maxCamAngleY)
			cameraAngle.y = maxCamAngleY;
		else if (cameraAngle.y <= minCamAngleY)
			cameraAngle.y = minCamAngleY;
	}

	//Calculate direction vectors of the camera angle
	float cx = cosf(cameraAngle.x);
	float sx = sinf(cameraAngle.x);

	float cx90 = cosf(cameraAngle.x + PI / 2);
	float sx90 = sinf(cameraAngle.x + PI / 2);

	float sy = sinf(cameraAngle.y);
	float cy = cosf(cameraAngle.y);

	float forwardX = cx * sy;
	float forwardY = cy;
	float forwardZ = sx * sy;

	if (!cursorEnabled) 
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
	//Place camera's target to the direction looking at.
	m_camera.target.x = m_camera.position.x + forwardX;
	m_camera.target.y = m_camera.position.y + forwardY;
	m_camera.target.z = m_camera.position.z + forwardZ;

	UpdateCamera(&m_camera);
}
//-----------------------------------------------------------------------------