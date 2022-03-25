#pragma once

#include "RenderObject.h"

//https://github.com/irlanrobson/bounce
//draw ray in debug
//https://www.youtube.com/watch?v=u0_-JDVVUlY

RenderObject m_object1;
RenderObject m_object2;
RenderObject m_object3;

bool rettestModel1 = false;
bool stopAnimTest = false;

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
}

void FrameTest()
{
	RenderObjectManager::Get()->Draw();
}

void UpdateTest()
{
	if (!stopAnimTest)
	{
		m_object1.Move({ 0.0f, 0.0f, (rettestModel1 ? 1.0f : -1.0f) });
		if (m_object1.GetPosition().z < 9.0f) rettestModel1 = true;
		if (m_object1.GetPosition().z > 20.0f) rettestModel1 = false;
	}

	RenderObjectManager::Get()->Update();
}