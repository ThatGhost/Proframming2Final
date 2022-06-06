#include "pch.h"
#include "E_Grum.h"
#include "GameVariables.h"

E_Grum::E_Grum() :
	Enemy("E_Grum.png", 10, 50, 2.f)
{}

void E_Grum::Attack(const float& elapsedSec)
{
	for (size_t i = 0; i < 3; i++)
	{
		double angle{ GetAngleBetweenPoints(g_Player->GetPosition(),m_Position) };// -M_PI / 4.0
		double randomAngle{ ((rand() % 314) / 100.f - M_PI / 2) * 0.2f };
		Vector2f dir{ (float)cos(angle + randomAngle), (float)sin(angle + randomAngle) };

		g_BulletManager->ShootBullet(m_Position, dir, 1, true);
	}
	g_SoundManager->PlaySound("FieryShoot.wav");
}