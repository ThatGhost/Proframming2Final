#include "pch.h"
#include "E_Mini.h"
#include "Image.h"
#include "GameVariables.h"

E_Mini::E_Mini():
	Enemy("E_Mini.png", 10, 50, 0.3f)
{}

void E_Mini::Attack(const float& elapsedSec)
{
	double angle{ GetAngleBetweenPoints(g_Player->GetPosition(),m_Position) };// -M_PI / 4.0
	double randomAngle{ rand() % 314 / 100.f - M_PI / 2 * 0.5f };

	Vector2f dir{ (float)cos(angle + randomAngle), (float)sin(angle + randomAngle) };

	g_BulletManager->ShootBullet(m_Position, dir, 1, true);
	g_SoundManager->PlaySound("FieryShoot.wav");
}