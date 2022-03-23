#include "stdafx.h"
#include "RenderObjectManager.h"
#include "RenderObject.h"
//-----------------------------------------------------------------------------
RenderObjectManager* gRenderObjectManager = nullptr;
//-----------------------------------------------------------------------------
RenderObjectManager::RenderObjectManager()
{
	assert(!gRenderObjectManager);
	gRenderObjectManager = this;
}
//-----------------------------------------------------------------------------
RenderObjectManager::~RenderObjectManager()
{
	gRenderObjectManager = nullptr;
}
//-----------------------------------------------------------------------------
RenderObjectManager* RenderObjectManager::Get()
{
	assert(gRenderObjectManager);
	return gRenderObjectManager;
}
//-----------------------------------------------------------------------------
void RenderObjectManager::AddRenderObject(RenderObject* object)
{
	auto it = std::find(m_renderObjects.begin(), m_renderObjects.end(), object);
	if (it == m_renderObjects.end())
		m_renderObjects.push_back(object);
}
//-----------------------------------------------------------------------------
void RenderObjectManager::Collisions(RenderObject* object, Vector3& newPos, Vector3& direction)
{
	const BoundingBox& currentBoundingBox = object->GetBoundingBox();
	for (int i = 0; i < m_renderObjects.size(); i++)
	{
		if (m_renderObjects[i] == object) continue;

		const BoundingBox& targetBoundingBox = m_renderObjects[i]->GetBoundingBox();

		// TODO: test AABB

		float outY;
		if (m_renderObjects[i]->CalculateY(object->m_position, outY))
		{
			if (outY > newPos.y)
				newPos.y = outY;
		}
	}
}
//-----------------------------------------------------------------------------
void RenderObjectManager::Draw()
{
	for (int i = 0; i < m_renderObjects.size(); i++)
	{
		m_renderObjects[i]->Draw();
	}
}
//-----------------------------------------------------------------------------
void RenderObjectManager::Update()
{
	for (int i = 0; i < m_renderObjects.size(); i++)
	{
		m_renderObjects[i]->Update();
	}
}
//-----------------------------------------------------------------------------