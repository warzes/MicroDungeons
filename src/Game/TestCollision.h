#pragma once

#include "RenderObject.h"

//Model testModel1;
//Vector3 testModel1Pos = { 10.0f, 0.0, 10.0f };
bool rettestModel1 = false;

RenderObject m_object1;
RenderObject m_object2;

bool stopAnimTest = false;

void InitTest()
{
	//testModel1 = LoadModelFromMesh(GenMeshCylinder(0.5f, 1.0f, 32.0f));

	m_object1.Init(LoadModelFromMesh(GenMeshCylinder(0.5f, 1.0f, 32.0f)));
	m_object1.SetPosition({ 10.0f, 0.0, 10.0f });


	m_object2.Init("../data/models/bridge.obj", "../data/models/bridge_diffuse.png");
	m_object2.SetScale(0.2f);
	m_object2.SetPosition({ 10.0f, 0.0f, 12.0f });
}

void FrameTest()
{
	m_object1.Draw();
	m_object2.Draw();
}

void UpdateTest()
{
	if (!stopAnimTest)
	{
		m_object1.Move({ 0.0f, 0.0f, (rettestModel1 ? 1.0f : -1.0f) });
		if (m_object1.GetPosition().z < 9.0f) rettestModel1 = true;
		if (m_object1.GetPosition().z > 14.0f) rettestModel1 = false;
	}

	m_object1.Update();
	m_object2.Update();

	//testModel1Pos.y = 0.0f;
	//Vector3 out;
	//if (m_object2.Collisions(testModel1Pos, out))
	//{
	//	testModel1Pos.y = out.y;
	//}
}