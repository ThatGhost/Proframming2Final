#include "pch.h"
#include "Bullet.h"
#include "DungeonDoor.h"
#include "Texture.h"
#include "Vector2f.h"
#include "GameVariables.h"
#include "utils.h"
#include "Image.h"

using namespace utils;

Bullet::Bullet(float damage, Texture* t) :
	GameObject("Bullet"),
	m_Damage{ (int)damage },
	m_Direction{Vector2f(0,0)},
	m_Speed{ 1.5f }
{
	m_Active = false;
	m_Sprite = t;
}

void Bullet::Draw() const
{
	m_Sprite->Draw(m_Position);
}

void Bullet::SetTrajectory(const Point2f& pos, const Vector2f& dir, const float& speed, const float& destroyTime)
{ 
	m_Position = pos; 
	m_Direction = dir; 
	m_Speed = speed; 
	m_Timer = destroyTime;
}

void Bullet::Update(const float& elapsedSec)
{
	if (m_Timer > 0) m_Timer -= elapsedSec;
	else {
		SetActive(false);
	}

	//collisions
	Rectf collider{ GetCollider() };

	if (m_FromEnemy) 
	{
		Rectf playerCollider{g_Player->GetCollider()};
		if (IsOverlapping(collider, playerCollider)) 
		{
			g_Player->Damage((int)m_Damage);
			SetActive(false);
		}
	}
	else
	{
		for (GameObject* const& o : g_AllBulletCollisionObjects)
		{
			if (o->IsActive()) 
			{
				Rectf ObjectCollider{ o->GetCollider() };
				if (IsOverlapping(ObjectCollider, collider)) 
				{
					o->Damage((int)m_Damage);
					SetActive(false);
					return;
				}
			}
		}
	}

	Point2f position{ collider.left - m_MyRoom->GetPosition().x * utils::g_SizeTiles,collider.bottom - m_MyRoom->GetPosition().y * utils::g_SizeTiles };
	Rectf newCollider{ position.x,position.y,collider.width,collider.height };
	bool isColliding{ false };
	Image* bounds{ g_Player->GetRoom()->GetImage() };
	//checking in a 3x3 grind around me to check for walls (red colordata == 33)
	//if the "future collider" collides with any of the walls then you cant move in that dir
	for (int y = -1; y < 2; y++)
	{
		for (int x = -1; x < 2; x++)
		{
			Point2i currentPosIdx{ (int)(position.x / utils::g_SizeTiles + x), (int)(position.y / utils::g_SizeTiles + y) };
			Rectf square{ (float)currentPosIdx.x * utils::g_SizeTiles, (float)currentPosIdx.y * utils::g_SizeTiles, utils::g_SizeTiles, utils::g_SizeTiles };
			if (utils::IsOverlapping(square, newCollider)) {
				if (bounds->data[((bounds->h - currentPosIdx.y) * bounds->w + (currentPosIdx.x)) * 3] == 33) {//33
					isColliding = true;
				}
			}
		}
	}
	for (GameObject* g : g_AllGameObjects) {
		if (g->name == "Door.png" || g->name == "DoorV.png") {
			if (utils::IsOverlapping(g->GetCollider(), collider))
			{
				isColliding = !static_cast<DungeonDoor*>(g)->IsUnlocked();
			}
		}
	}
	if (isColliding)
	{
		SetActive(false);
	}

	//movement
	m_Position += Point2f(m_Direction.x * m_Speed * elapsedSec, m_Direction.y * m_Speed * elapsedSec);
}