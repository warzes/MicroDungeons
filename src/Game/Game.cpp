#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Enemy.h"
//-----------------------------------------------------------------------------
//rlFPCamera cam;
static Model boxModel1;
static Model boxModel2;
Texture2D texture;
BoundingBox bounds;

CollisionModel3D colModel1, colModel2;
float offset = 2.0f;
bool ret = false;
bool stopAnim = true;

static const Vector2 posModel = { 3.0f, 3.0f };
//-----------------------------------------------------------------------------
Game::Game()
	: m_playerLogic(m_playerCamera)
{
	//cam.Setup(45, Vector3{ 0, 10, 0 });
	//cam.MoveSpeed.z = 10;
	//cam.MoveSpeed.x = 5;
	//cam.FarPlane = 5000;

	m_playerCamera.Init({ 5.0f, 0.0f, 10.0f }, 180, 90);
	m_playerAnimation.Init();
	m_world.Init();	

	boxModel1 = LoadModelFromMesh(GenMeshCylinder(0.4f, 0.6f, 32));
	//boxModel1 = LoadModelFromMesh(GenMeshCube(0.5f, 0.5f, 0.5f));
	boxModel2 = LoadModel("well.obj");
	texture = LoadTexture("well_diffuse.png");
	boxModel2.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
	bounds = GetMeshBoundingBox(boxModel2.meshes[0]);

	for (int i = 0; i < boxModel1.meshCount; i++)
	{
		const auto& mesh = boxModel1.meshes[i];
		std::vector<Vector3> vertices;
		vertices.reserve(mesh.vertexCount);
		for (int a = 0; a < mesh.vertexCount * 3; a += 3)
		{
			vertices.push_back( { mesh.vertices[a + 0], mesh.vertices[a + 1], mesh.vertices[a + 2] });
			//std::cout << vertices[i].x << ":" << vertices[i].y << ":" << vertices[i].z << std::endl;
		}
		if (mesh.triangleCount > 0 && mesh.indices)
		{
			size_t numIndex = mesh.triangleCount * 3;
			for (int a = 0; a < numIndex; a+=3)
			{
				int index1 = mesh.indices[a + 0];
				int index2 = mesh.indices[a + 1];
				int index3 = mesh.indices[a + 2];

				Vector3 point1 = vertices[index1];
				Vector3 point2 = vertices[index2];
				Vector3 point3 = vertices[index3];

				colModel1.addTriangle(
					point1.x, point1.y, point1.z,
					point2.x, point2.y, point2.z,
					point3.x, point3.y, point3.z);
			}
		}
		else
		{
			/*for (int j = 0; j < mesh.vertexCount; j += 9)
			{
				Vector3 point1, point2, point3;
				point1.x = mesh.vertices[j + 0];
				point1.y = mesh.vertices[j + 1];
				point1.z = mesh.vertices[j + 2];

				point2.x = mesh.vertices[j + 3];
				point2.y = mesh.vertices[j + 4];
				point2.z = mesh.vertices[j + 5];

				point3.x = mesh.vertices[j + 6];
				point3.y = mesh.vertices[j + 7];
				point3.z = mesh.vertices[j + 8];

				colModel1.addTriangle(
					point1.x, point1.y, point1.z,
					point2.x, point2.y, point2.z,
					point3.x, point3.y, point3.z);
			}*/
		}		
	}
	colModel1.finalize();


	for (int i = 0; i < boxModel2.meshCount; i++)
	{
		const auto& mesh = boxModel2.meshes[i];
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

				Vector3 point1 = vertices[index1];
				Vector3 point2 = vertices[index2];
				Vector3 point3 = vertices[index3];

				colModel2.addTriangle(
					point1.x, point1.y, point1.z,
					point2.x, point2.y, point2.z,
					point3.x, point3.y, point3.z);
			}
		}
		else
		{
			for (int a = 0; a < vertices.size(); a+=3)
			{
				Vector3 point1 = vertices[a + 0];
				Vector3 point2 = vertices[a + 1];
				Vector3 point3 = vertices[a + 2];
				colModel2.addTriangle(
					point1.x, point1.y, point1.z,
					point2.x, point2.y, point2.z,
					point3.x, point3.y, point3.z);
			}



			/*for (int j = 0; j < mesh.vertexCount; j += 9)
			{
				Vector3 point1, point2, point3;
				point1.x = mesh.vertices[j + 0];
				point1.y = mesh.vertices[j + 1];
				point1.z = mesh.vertices[j + 2];

				point2.x = mesh.vertices[j + 3];
				point2.y = mesh.vertices[j + 4];
				point2.z = mesh.vertices[j + 5];

				point3.x = mesh.vertices[j + 6];
				point3.y = mesh.vertices[j + 7];
				point3.z = mesh.vertices[j + 8];

				colModel2.addTriangle(
					point1.x, point1.y, point1.z,
					point2.x, point2.y, point2.z,
					point3.x, point3.y, point3.z);
			}*/
		}
	}
	colModel2.finalize();

	//Matrix matScale = MatrixScale(1.0f, 1.0f, 1.0f);
	//Matrix matRotation = MatrixRotate({ 0.0f, 1.0f, 0.0f }, 0.0f * DEG2RAD);
	//Matrix matTranslation = MatrixTranslate(posModel.x, 0.5f, posModel.y);
	//Matrix matTransform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);
	//Matrix World = MatrixTranspose(matTransform);
	//const float m1[] = {
	//	World.m0, World.m4, World.m8,  World.m12,
	//	World.m1, World.m5, World.m9,  World.m13,
	//	World.m2, World.m6, World.m10, World.m14,
	//	World.m3, World.m7, World.m11, World.m15
	//};

	//colModel1.setTransform(&World.m0);

	//matScale = MatrixScale(0.15f, 0.15f, 0.15f);
	//matRotation = MatrixRotate({ 0.0f, 1.0f, 0.0f }, 0.0f * DEG2RAD);
	//matTranslation = MatrixTranslate(posModel.x + offset, 0.0f, posModel.y);
	//matTransform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);
	//World = MatrixTranspose(matTransform);
	//const float m2[] = {
	//	World.m0, World.m4, World.m8,  World.m12,
	//	World.m1, World.m5, World.m9,  World.m13,
	//	World.m2, World.m6, World.m10, World.m14,
	//	World.m3, World.m7, World.m11, World.m15
	//};
	//colModel2.setTransform(&World.m0);

	//ModelCollisionTest modelTest;
	//modelTest.setOtherModel(&colModel2);
	////modelTest.setOtherModelTransform(m2);
	//if (colModel1.modelCollision(&modelTest))
	//{
	//	std::cout << "collision" << std::endl;
	//}

	
	m_enemyManager.Init();
}
//-----------------------------------------------------------------------------
void Game::Frame()
{
	BeginDrawing();
	//ClearBackground({ 140, 210, 240 });
	ClearBackground({ 50, 100, 200 });

	//cam.BeginMode3D();
	BeginMode3D(m_playerCamera.GetCamera());
	{
		//DrawPlane(Vector3{ 0, 0, 0 }, Vector2{ 50, 50 }, BEIGE);
		m_world.Frame();

		DrawModel(boxModel1, { posModel.x, 0.2f, posModel.y }, 1.0f, GOLD);
		DrawModel(boxModel2, { posModel.x+ offset, 1.0f, posModel.y }, 0.3f, DARKGRAY);

		m_enemyManager.Draw();
	}
	EndMode3D();
	//cam.EndMode3D();
	m_playerAnimation.Draw2D();

	DrawFPS(0, 0);
	EndDrawing();
}
//-----------------------------------------------------------------------------
void Game::Update()
{
	if (!stopAnim)
	{
		offset += 0.5 * GetFrameTime() * (ret ? 1 : -1);
		if (offset < -2 || offset > 2)
			ret = !ret;
	}



	Matrix matScale = MatrixScale(1.0f, 1.0f, 1.0f);
	Matrix matRotation = MatrixRotate({ 0.0f, 1.0f, 0.0f }, 0.0f * DEG2RAD);
	Matrix matTranslation = MatrixTranslate(posModel.x, 0.2f, posModel.y);
	Matrix matTransform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);
	Matrix world = MatrixTranspose(matTransform);
	colModel1.setTransform(&world.m0);

	matScale = MatrixScale(0.3f, 0.3f, 0.3f);
	matRotation = MatrixRotate({ 0.0f, 1.0f, 0.0f }, 0.0f * DEG2RAD);
	matTranslation = MatrixTranslate(posModel.x + offset, 1.0f, posModel.y);
	matTransform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);
	world = MatrixTranspose(matTransform);
	colModel2.setTransform(&world.m0);

	ModelCollisionTest modelTest;
	modelTest.setOtherModel(&colModel2);
	//modelTest.setOtherModelTransform(m2);
	//if (colModel1.modelCollision(&modelTest))
	if (m_playerCamera.TestCollision(&modelTest))
	{
		std::cout << "collision" << std::endl;
		stopAnim = true;
	}
	else
		std::cout << "no collision" << std::endl;


	/*auto pos = m_playerCamera.GetPosition();
	pos.y = 10;
	cam.SetCameraPosition(pos);
	cam.Update();*/

	// Player logic
	m_playerCamera.Update(m_world, m_enemyManager, &modelTest, GetFrameTime());
	m_playerAnimation.Update(GetFrameTime());

	// Enemy logic
	m_enemyManager.Update(m_playerCamera, m_world, GetFrameTime());

	// Player Attack Events
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		m_playerAnimation.StartAnimationAttacking();
	}

	// Attack logic
	if (m_playerAnimation.IsEndAnimationAttack())
	{
		const Vector2 rayMousePos = { static_cast<float>(GetScreenWidth() / 2), static_cast<float>(GetScreenHeight() / 2) };
		const Ray ray = GetMouseRay(rayMousePos, m_playerCamera.GetCamera());
		m_enemyManager.PlayerAttack(m_playerCamera.GetCamera().position, ray);
	}
}
//-----------------------------------------------------------------------------
void Game::Close()
{
	m_enemyManager.Close();
	m_playerAnimation.Close();
	m_world.Close();
}
//-----------------------------------------------------------------------------