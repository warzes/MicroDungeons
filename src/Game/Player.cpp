#include "stdafx.h"
#include "Player.h"
#include "World.h"
#include "EnemyManager.h"
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

	//m_collisionBox.min = { 0.0f, 0.0f, 0.0f };
	//m_collisionBox.max = { 0.4f, 0.6f, 0.4f };
	m_collisionBox.min = { -0.2f, 0.0f, -0.2f };
	m_collisionBox.max = {  0.2f, 0.6f,  0.2f };
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
	m_position = { startPosition.x, startPosition.y+0.1f, startPosition.z };
	this->DisableCursor();
	m_cameraAngle.x = (float)rotationCamX * DEG2RAD;
	m_cameraAngle.y = (float)rotationCamY * DEG2RAD;

	Model boxModel = LoadModelFromMesh(GenMeshCylinder(0.4f, 0.56f, 64)); // todo: optimaze
	for (int i = 0; i < boxModel.meshCount; i++)
	{
		const auto& mesh = boxModel.meshes[i];
		std::vector<Vector3> vertices;
		vertices.reserve(mesh.vertexCount);
		for (int a = 0; a < mesh.vertexCount * 3; a += 3)
		{
			vertices.push_back({ mesh.vertices[a + 0], mesh.vertices[a + 1], mesh.vertices[a + 2] });
			//std::cout << vertices[i].x << ":" << vertices[i].y << ":" << vertices[i].z << std::endl;
		}
		if (mesh.triangleCount > 0 && mesh.indices)
		{
			size_t numIndex = mesh.triangleCount * 3;
			for (int a = 0; a < numIndex; a += 3)
			{
				int index1 = mesh.indices[a + 0];
				int index2 = mesh.indices[a + 1];
				int index3 = mesh.indices[a + 2];

				const Vector3& point1 = vertices[index1];
				const Vector3& point2 = vertices[index2];
				const Vector3& point3 = vertices[index3];
				m_colModel.addTriangle(
					point1.x, point1.y, point1.z,
					point2.x, point2.y, point2.z,
					point3.x, point3.y, point3.z);
			}
		}
		else
		{
			for (int a = 0; a < vertices.size(); a += 3)
			{
				const Vector3& point1 = vertices[a + 0];
				const Vector3& point2 = vertices[a + 1];
				const Vector3& point3 = vertices[a + 2];
				m_colModel.addTriangle(
					point1.x, point1.y, point1.z,
					point2.x, point2.y, point2.z,
					point3.x, point3.y, point3.z);
			}
		}
	}
	m_colModel.finalize();


	UnloadModel(boxModel);
}
//-----------------------------------------------------------------------------
void PlayerCamera::Update(const World& world, const EnemyManager& enemyManager, ModelCollisionTest* modelCollision, float deltaTime)
{
	//Gravity
	m_velocity.y -= 0.012f * (deltaTime * 60.0f);

	//Calculate velocity with delta time
	Vector3 velXdt = Vector3Scale(m_velocity, deltaTime * 60.0f);

	m_position.x += velXdt.x;
	if (TestCollision(world, enemyManager, modelCollision)) m_position.x -= velXdt.x;
	m_position.z += velXdt.z;
	if (TestCollision(world, enemyManager, modelCollision)) m_position.z -= velXdt.z;

	m_position.y += velXdt.y;
	if (TestCollision(world, enemyManager, modelCollision) || m_position.y < 0.0f)
	{
		m_position.y -= velXdt.y;
		if (m_velocity.y <= 0.0f) m_canJump = true;
		m_velocity.y = 0.0f;
	}

	//std::cout << "pos " << m_position.x << ":" << m_position.y << ":" << m_position.z << std::endl;

	m_camera.position = m_position;
	m_camera.position.y += 0.5f;

	m_velocity.x -= velXdt.x / 6.0f;
	m_velocity.z -= velXdt.z / 6.0f;

	checkInputs();
}
//-----------------------------------------------------------------------------
bool PlayerCamera::TestCollision(const World& world, const EnemyManager& enemyManager, ModelCollisionTest* modelCollision)
{
	if (world.TestCollision(GetBoundingBox())) return true;
	if (enemyManager.TestCollision(m_position, GetBoundingBox())) return true;

	Matrix matScale = MatrixScale(1.0f, 1.0f, 1.0f);
	Matrix matRotation = MatrixRotate({ 0.0f, 1.0f, 0.0f }, 0.0f * DEG2RAD);
	Matrix matTranslation = MatrixTranslate(m_position.x, m_position.y+0.3, m_position.z);
	Matrix matTransform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);
	Matrix matWorld = MatrixTranspose(matTransform);
	m_colModel.setTransform(&matWorld.m0);
	if (TestCollision(modelCollision))
	{
		auto t = modelCollision->modelTriangle();
		std::cout << t << std::endl;
		todo
		return true;
	}

	

	return false;
}
//-----------------------------------------------------------------------------
bool PlayerCamera::TestCollision(ModelCollisionTest* modelCollision)
{
	return m_colModel.modelCollision(modelCollision);
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
	SetMousePosition(GetScreenWidth() / 2, GetScreenHeight() / 2);
}
//-----------------------------------------------------------------------------
BoundingBox PlayerCamera::GetBoundingBox() const
{
	BoundingBox pB = m_collisionBox;
	pB.min = Vector3Add(pB.min, m_position);
	pB.max = Vector3Add(pB.max, m_position);
	return pB;
}
//-----------------------------------------------------------------------------
Vector3 PlayerCamera::GetPosition() const
{
	return m_position;
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
PlayerLogic::PlayerLogic(PlayerCamera& playerCamera)
	: m_position(playerCamera.m_position)
{
}
//-----------------------------------------------------------------------------