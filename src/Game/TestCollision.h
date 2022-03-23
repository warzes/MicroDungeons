#pragma once

Model testModel1;
Vector3 testModel1Pos = { 10, 0.0, 10 };
float offsetZtestModel1 = 0.0f;
float offsetYtestModel1 = 0.0f;
bool rettestModel1 = false;

Model testModel2;
Vector3 testModel2Pos = { 10, 0.0, 12 };
float scaletestModel2 = 0.2f;
BoundingBox boundstestModel2;
Texture2D textureTestModel2;

bool stopAnimTest = false;


void InitTest()
{
	testModel1 = LoadModelFromMesh(GenMeshCylinder(0.5f, 1.0f, 32));
	testModel2 = LoadModelFromMesh(GenMeshCylinder(0.5f, 0.5f, 32));
	testModel2 = LoadModel("../data/models/bridge.obj");
	textureTestModel2 = LoadTexture("../data/models/bridge_diffuse.png");
	testModel2.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = textureTestModel2;
	boundstestModel2 = GetMeshBoundingBox(testModel2.meshes[0]);
}

void FrameTest()
{
	Vector3 pos = testModel1Pos;
	pos.z += offsetZtestModel1;
	pos.y = offsetYtestModel1;
	DrawModel(testModel1, pos, 1.0f, DARKPURPLE);

	pos = testModel2Pos;
	DrawModel(testModel2, pos, scaletestModel2, GREEN);

	//DrawModel(testModel2, pos, scaletestModel2, WHITE);
	//boundstestModel2 = GetMeshBoundingBox(testModel2.meshes[0]);
	//DrawBoundingBox(boundstestModel2, LIME);
}

void UpdateTest()
{
	if (!stopAnimTest)
	{
		offsetZtestModel1 += 1.5 * GetFrameTime() * (rettestModel1 ? 1 : -1);
		if (offsetZtestModel1 < -2.0|| offsetZtestModel1 > 4)
			rettestModel1 = !rettestModel1;
	}

	const Vector3 NegativeUnitY = { 0.0f, -1.0f, 0.0f };


	Vector3 curPos = testModel1Pos;
	curPos.z += offsetZtestModel1;
	curPos.y = 50.5;

	Ray ray = {0};
	ray.position = curPos;
	ray.direction = NegativeUnitY;
	boundstestModel2 = GetMeshBoundingBox(testModel2.meshes[0]);
	boundstestModel2.min = Vector3Scale(boundstestModel2.min, scaletestModel2);
	boundstestModel2.max = Vector3Scale(boundstestModel2.max, scaletestModel2);
	boundstestModel2.min = Vector3Add(boundstestModel2.min, testModel2Pos);
	boundstestModel2.max = Vector3Add(boundstestModel2.max, testModel2Pos);

	offsetYtestModel1 = 0.0f;
	RayCollision boxHitInfo = GetRayCollisionBox(ray, boundstestModel2);
	if ((boxHitInfo.hit) && (boxHitInfo.distance < 100))
	{
		//std::cout << "test1" << std::endl;
		Matrix matScale = MatrixScale(scaletestModel2, scaletestModel2, scaletestModel2);
		Matrix matRotation = MatrixRotate({ 0.0f, 1.0f, 0.0f }, 0.0f * DEG2RAD);
		Matrix matTranslation = MatrixTranslate(testModel2Pos.x, testModel2Pos.y, testModel2Pos.z);
		Matrix matTransform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);
		//Matrix oldmatTransform = testModel2.transform;
		//testModel2.transform = matTranslation;
		//RayCollision meshHitInfo = GetRayCollisionModel(ray, testModel2);
		RayCollision meshHitInfo = GetRayCollisionMesh(ray, testModel2.meshes[0], matTransform);
		if (meshHitInfo.hit)
		{
			//std::cout << "test2" << std::endl;

			offsetYtestModel1 = meshHitInfo.point.y;
		}
		//testModel2.transform = oldmatTransform;

	}
}