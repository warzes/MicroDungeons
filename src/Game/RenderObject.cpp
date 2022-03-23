#include "stdafx.h"
#include "RenderObject.h"
#include "RenderObjectManager.h"
//
//moc - collision

//-----------------------------------------------------------------------------
const Vector3 NegativeUnitY = { 0.0f, -1.0f, 0.0f };
//-----------------------------------------------------------------------------
void RenderObject::Init(const Model& model)
{
	m_model = model;
	m_bounds = GetMeshBoundingBox(m_model.meshes[0]);

	auto manager = RenderObjectManager::Get();
	manager->AddRenderObject(this);
}
//-----------------------------------------------------------------------------
void RenderObject::Init(const std::string& modelFileName, const std::string& diffuseTextureFileName)
{
	m_model = LoadModel(modelFileName.c_str());
	m_diffuseTexture = LoadTexture(diffuseTextureFileName.c_str());
	m_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = m_diffuseTexture;
	m_bounds = GetMeshBoundingBox(m_model.meshes[0]);

	auto manager = RenderObjectManager::Get();
	manager->AddRenderObject(this);
}
//-----------------------------------------------------------------------------
void RenderObject::SetScale(float scale)
{
	m_scale = scale;
}
//-----------------------------------------------------------------------------
void RenderObject::SetPosition(const Vector3& pos)
{
	m_position = pos;
}
//-----------------------------------------------------------------------------
void RenderObject::Move(const Vector3& direction)
{
	m_direction.x = std::clamp(direction.x, -1.0f, 1.0f);
	m_direction.y = std::clamp(direction.y, -1.0f, 1.0f);
	m_direction.z = std::clamp(direction.z, -1.0f, 1.0f);
}
//-----------------------------------------------------------------------------
void RenderObject::Update()
{
	m_bounds = GetMeshBoundingBox(m_model.meshes[0]);
	m_bounds.min = Vector3Scale(m_bounds.min, m_scale);
	m_bounds.max = Vector3Scale(m_bounds.max, m_scale);
	m_bounds.min = Vector3Add(m_bounds.min, m_position);
	m_bounds.max = Vector3Add(m_bounds.max, m_position);

	Matrix matScale = MatrixScale(m_scale, m_scale, m_scale);
	Matrix matRotation = MatrixRotate({ 0.0f, 1.0f, 0.0f }, 0.0f * DEG2RAD);
	Matrix matTranslation = MatrixTranslate(m_position.x, m_position.y, m_position.z);
	m_matTransform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

	// Move and collisions
	if (m_direction.x == 0.0f && m_direction.y == 0.0f && m_direction.z == 0.0f) return;

	Vector3 newPos;
	newPos.x = m_position.x + 1.5f * GetFrameTime() * m_direction.x;
	newPos.y = m_position.y + 1.5f * GetFrameTime() * m_direction.y;
	newPos.z = m_position.z + 1.5f * GetFrameTime() * m_direction.z;

	RenderObjectManager::Get()->Collisions(this, newPos);

	m_position = newPos;
}
//-----------------------------------------------------------------------------
void RenderObject::Draw()
{
	DrawModel(m_model, m_position, m_scale, WHITE);
	DrawBoundingBox(m_bounds, LIME);
}
//-----------------------------------------------------------------------------
bool RenderObject::CalculateY(const Vector3& point, float& outY)
{
	Vector3 pos = point;
	pos.y += 0.5f;

	Ray ray = { 0 };
	ray.position = pos;
	ray.direction = NegativeUnitY;
	RayCollision boxHitInfo = GetRayCollisionBox(ray, m_bounds);
	if ((boxHitInfo.hit) && (boxHitInfo.distance < 1))
	{		
		RayCollision meshHitInfo = GetRayCollisionMesh(ray, m_model.meshes[0], m_matTransform);
		if (meshHitInfo.hit)
		{
			outY = meshHitInfo.point.y;
			return true;
		}
	}
	return false;
}
//-----------------------------------------------------------------------------