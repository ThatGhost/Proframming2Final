#include "pch.h"
#include "BulletManager.h"
#include "GameVariables.h"
#include "Vector2f.h"
#include "Texture.h"
#include "Bullet.h"

BulletManager::BulletManager()
{
	m_BulletTexture = new Texture("Resources/objects/50cal.png");
	for (int i{}; i < 25; i++)
	{
		Bullet* bullet{ new Bullet(10, m_BulletTexture) };
		m_Bullets.push_back(bullet);
		bullet->SetRoom(g_Player->GetRoom());
	}
}

BulletManager::~BulletManager()
{
	delete m_BulletTexture;
	for (Bullet* b : m_Bullets)
	{
		delete b;
	}
}

Bullet* BulletManager::GetBullet()
{
	//find a bullet that is not in use
	//if no bullets -> make more and give last
	for (Bullet* b : m_Bullets)
	{
		if (!b->IsActive())return b;
	}
	Resize();

	return m_Bullets[m_Bullets.size() - 1];
}

void BulletManager::ShootBullet(const Point2f& pos, const Vector2f& dir, const int& att, const bool& isEnemy)
{
	Bullet* b = GetBullet();

	b->SetActive(true);
	b->SetFromEnemy(isEnemy);
	b->SetDamage(att);
	b->SetRoom(g_Player->GetRoom());

	if(isEnemy)b->SetTrajectory(pos,dir,75);
	else b->SetTrajectory(pos, dir, 500);
}

void BulletManager::DrawBullets() const
{
	for (Bullet* b : m_Bullets)
	{
		if(b->IsActive())b->Draw();
	}
}

void BulletManager::UpdateBullets(const float& elapsedSec)
{
	for (Bullet* b : m_Bullets)
	{
		if (b->IsActive())b->Update(elapsedSec);
	}
}

void BulletManager::Resize()
{
	for (size_t i = 0; i < 10; i++)
	{
		Bullet* bullet{ new Bullet(10, m_BulletTexture) };
		m_Bullets.push_back(bullet);
		bullet->SetRoom(g_Player->GetRoom());
	}
}

void BulletManager::Destruct()
{
	delete m_BulletTexture;
	for (Bullet* b : m_Bullets)
	{
		delete b;
	}
}
