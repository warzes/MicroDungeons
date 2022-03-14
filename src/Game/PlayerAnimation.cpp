#include "stdafx.h"
#include "PlayerAnimation.h"

static Texture2D sword;
static Rectangle frameRec;
static Vector2 swordPosition = { 340.0f, 120.0f };
static Vector2 swordOffsetPosition = { 340.0f, 120.0f };
static bool attacking = false;
static int currentFrame = 0;
static int framesCounter = 0;
static int attackCounter = 0;
#define NUM_FRAMES_PER_LINE     4

void PlayerAnimation::Init()
{
	swordOffsetPosition = { GetScreenWidth() - 800.0f, GetScreenHeight() - 430.0f };
	swordPosition.x += swordOffsetPosition.x;
	swordPosition.y += swordOffsetPosition.y;

	//swordPosition = { 564.0f, 420.0f };
	sword = LoadTexture("../data/textures/sword.png");
	frameRec.x = 0.0f;
	frameRec.y = 0.0f;
	frameRec.width = (float)sword.width / 4;
	frameRec.height = (float)sword.height / 2;
}

void PlayerAnimation::Close()
{
}

void PlayerAnimation::Draw2D()
{
	DrawTextureRec(sword, frameRec, swordPosition, WHITE);
}

void PlayerAnimation::Update(Camera cam)
{
	// Sword moves with camera

	swordPosition.y = 50.0f * cam.target.y;
	swordPosition.x = 340.0f;
	if (cam.target.y <= 2.5f)
	{
		swordPosition.y += 30.0f;
	}
	if (cam.target.y <= 1.5f)
	{
		swordPosition.y += 25.0f;
	}
	swordPosition.x += swordOffsetPosition.x;
	swordPosition.y += swordOffsetPosition.y;

	framesCounter++;

	if (attacking)
	{
		frameRec.y = 400.0f;
		frameRec.x = 400.0f * currentFrame;
		swordPosition.x = 240.0f;
		swordPosition.y = 145.0f;
		swordPosition.x += swordOffsetPosition.x;
		swordPosition.y += swordOffsetPosition.y;
		attackCounter++;
		if (attackCounter > 3)
		{
			currentFrame++;
			if (currentFrame >= NUM_FRAMES_PER_LINE)
			{
				currentFrame = 0;
				attacking = false;

			
				////posModel = { m_playerCamera.GetCamera().target.x, m_playerCamera.GetCamera().target.z };

				//Ray ray = { 0 };        // Picking ray
				//Vector2 rayMousePos = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
				////ray = GetMouseRay(GetMousePosition(), m_playerCamera.GetCamera());
				//ray = GetMouseRay(rayMousePos, cam);

				////Vector3 sp = { -30.0f, 5.0f, 5.0f };
				////float sr = 4.0f;
				////RayCollision sphereHitInfo = GetRayCollisionSphere(ray, sp, sr);
				//BoundingBox towerBBox;
				//towerBBox.min = { posModel.x - 0.5f, 0.0f, posModel.y - 0.5f };
				//towerBBox.max = { posModel.x + 0.5f, 1.0f, posModel.y + 0.5f };
				//RayCollision boxHitInfo = GetRayCollisionBox(ray, towerBBox);
				//if ((boxHitInfo.hit) && (boxHitInfo.distance < 0.3))
				//{
				//	std::cout << "hit=" << boxHitInfo.distance << "player pos=" << cam.position.x << ":" << m_playerCamera.GetCamera().position.z << std::endl;
				//}
				//else
				//{
				//	std::cout << "no hit" << std::endl;
				//}
			}
			attackCounter = 0;
		}
	}
	else
	{
		frameRec.y = 0.0f;
		frameRec.x = 0.0f;
	}
}

void PlayerAnimation::StartAttacking()
{
	attacking = true;
}

bool PlayerAnimation::IsAttack() const
{
	return attacking;
}