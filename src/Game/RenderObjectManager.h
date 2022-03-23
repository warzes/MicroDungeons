#pragma once

class RenderObject;

class RenderObjectManager
{
public:
	RenderObjectManager();
	~RenderObjectManager();

	static RenderObjectManager* Get();

	void AddRenderObject(RenderObject* object);

	void Collisions(RenderObject* object, Vector3& newPos);

private:
	std::vector<RenderObject*> m_renderObjects;
};