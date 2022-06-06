#include "pch.h"
#include "E_Fiery.h"
#include "GameVariables.h"

E_Fiery::E_Fiery() :
	Enemy("E_Fiery.png", 25, 50, 2.f)
{}

void E_Fiery::Attack(const float& elapsedSec)
{
	const int amountOfBUllets{ 8 };
	const float angleStep{2*(float)M_PI/amountOfBUllets};
	float angle{rand() % 314 / 100.f};

	for (int i{}; i < amountOfBUllets; i++)
	{
		Vector2f dir{cos(angle),sin(angle)};
		angle += angleStep;
		g_BulletManager->ShootBullet(m_Position,dir,1,true);
		g_SoundManager->PlaySound("FieryShoot.wav");
	}
}