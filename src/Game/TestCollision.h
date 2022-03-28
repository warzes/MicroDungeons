#pragma once

#include "RenderObject.h"
#include "BSP.h"

//https://github.com/irlanrobson/bounce
//draw ray in debug
//https://www.youtube.com/watch?v=u0_-JDVVUlY



new temp test

https ://www.youtube.com/watch?v=vZ9jdy_EZDA
https://www.geometrictools.com
softpixelengine

http ://www.peroxide.dk/papers/collision/collision.pdf

https://github.com/Pierre-Terdiman/PEEL_PhysX_Edition



RenderObject m_object1;
RenderObject m_object2;
RenderObject m_object3;

bool rettestModel1 = false;
bool stopAnimTest = false;

Model model1;
Model model2;

Vector3 camPos1 = { 0, 0, 0 };
Vector3 camPos2 = { 0, 0, -10 };

BSP bsp;

void InitTest()
{
	m_object1.Init(ObjectType::Dynamic, LoadModelFromMesh(GenMeshCylinder(0.5f, 1.0f, 32.0f)));
	m_object1.SetPosition({ 10.0f, 0.0, 10.0f });

	/*m_object2.Init(ObjectType::Static, "../data/models/bridge.obj", "../data/models/bridge_diffuse.png");
	m_object2.SetScale(0.2f);
	m_object2.SetPosition({ 10.0f, 0.0f, 12.0f });*/

	//m_object3.Init(ObjectType::Static, LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f)));
	m_object3.Init(ObjectType::Static, "../data/models/well.obj", "../data/models/well_diffuse.png");
	m_object3.SetPosition({ 10.0f, 0.0, 16.0f });

	model1 = LoadModel("../data/models/untitled.obj");
	for (int i = 0; i < model1.meshCount; i++)
	{
		const auto& mesh = model1.meshes[i];
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
				
				bsp.AddTriangle(point1, point2, point3);
			}
		}
		else
		{
			for (int a = 0; a < vertices.size(); a += 3)
			{
				const Vector3& point1 = vertices[a + 0];
				const Vector3& point2 = vertices[a + 1];
				const Vector3& point3 = vertices[a + 2];

				bsp.AddTriangle(point1, point2, point3);
			}
		}
	}
	bsp.Build();

	model2 = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
}

void FrameTest()
{
	RenderObjectManager::Get()->Draw();

	DrawModel(model1, camPos1, 1.0f, RED);
	DrawModel(model2, camPos2, 1.0f, WHITE);
}

bool collision(const Vector3& p0, const Vector3& p1, Vector3* point, Vector3* normal, float* offset = nullptr)
{
	const BTri* tri;
	if (bsp.Intersects(p0, p1, point, &tri)) 
	{
		//		if (normal) *normal = tri->normal;
		//		if (offset) *offset = tri->offset;
		if (normal) *normal = { tri->plane.x, tri->plane.y, tri->plane.z };
		if (offset) *offset = tri->plane.w;
		return true;
	}
	return false;
}

void UpdateTest()
{
	if (!stopAnimTest)
	{
		m_object1.Move({ 0.0f, 0.0f, (rettestModel1 ? 1.0f : -1.0f) });
		if (m_object1.GetPosition().z < 9.0f) rettestModel1 = true;
		if (m_object1.GetPosition().z > 20.0f) rettestModel1 = false;
	}

	Vector3 Dir = {};

	if (IsKeyDown(KEY_E))
	{
		Dir = { 0.0f, 0.0f, -1.0f };
	}	
	if (IsKeyDown(KEY_Q))
	{
		Dir = { 0.0f, 0.0f, 1.0f };		
	}

	if (Dir.z != 0.0f)
	{
		Vector3 point = { 0,0,0 };
		//Vector3 normal = { 0,0,0 };

		//static bool walking = false;
		//float walkHeight = walking ? 0.0f : 0.0f;

		//if (walking = collision(camPos2, camPos2 - Vector3(0, walkHeight, 0), &point, &normal))
		//{
		//	// Walking
		//	camPos2 = point;
		//}
		//else
		//{

		//}








		Vector3 newPos = camPos2 + Dir *(2 * GetFrameTime());
		


		const BTri* tri;
		if (bsp.Intersects(camPos2, newPos, &point, &tri))
		{
			newPos = point + Vector3{ tri->plane.x, tri->plane.y, tri->plane.z };
		}
		else
		{
			newPos.y -= 2.5*GetFrameTime();	
		}
		if (newPos.y < 0) newPos.y = 0.0f;

		bsp.PushSphere(newPos, 0.5);

		camPos2 = newPos;		
	}

	RenderObjectManager::Get()->Update();
}