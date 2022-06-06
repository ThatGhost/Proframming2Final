#include "pch.h"
#include "E_Richard.h"
#include "GameVariables.h"

E_Richard::E_Richard() :
	Enemy("E_Richard.png", 25, 50, 2.f)
{}

void E_Richard::Attack(const float& elapsedSec)
{
	const int amountOfBUllets{ 5 };
	const float angleStep{ (float)M_PI / 2 / amountOfBUllets };
	
	double angle{ GetAngleBetweenPoints(g_Player->GetPosition(),m_Position) - M_PI / 4.0};

	for (int i{}; i < amountOfBUllets; i++)
	{
		Vector2f dir{ (float)cos(angle), (float)sin(angle) };
		angle += angleStep;
		g_BulletManager->ShootBullet(m_Position, dir, 1, true);
	}
}