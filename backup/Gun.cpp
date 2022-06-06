#include "pch.h"
#include "Gun.h"
#include "GameVariables.h"
#include <fstream>
#include <sstream>

Gun::Gun(const std::string& gunName)
	: GameObject("gun"),
	m_GunName{gunName}
{
	std::ifstream iStream{"resources/guns/gunstats.txt"};
	std::string line{};
	if (iStream.is_open()) {
		while (std::getline(iStream, line))
		{
			if (line[0] == gunName[0] && line[1] == gunName[1] && line[2] == gunName[2])
			{
				//same gun as in the file since every gun has a unique first 3 letters
				break;
			}
		}
	}

	std::istringstream iss(line);
	int l{};
	do
	{
		std::string subs;
		iss >> subs;
		if (l == 1) m_Att = std::stoi(subs);
		if (l == 2) m_ShootSpeed = std::stof(subs);
		if (l == 3) m_MaxAmmo = std::stoi(subs);
		if (l == 4) m_Maxbullets = std::stoi(subs);
		l++;
	} while (iss);

	m_Bullets = m_Maxbullets;
	m_AmmoInClip = m_MaxAmmo;

	iStream.close();
	m_Sprite = new Texture("resources/guns/"+gunName+".png");
}

Gun::~Gun()
{
	delete m_Sprite;
}

void Gun::Draw(const float& angle, const Point2f& pos)
{
	glPushMatrix();
	{
		glTranslatef(pos.x, pos.y - 10, 0);
		glRotatef(angle, 0, 0, 1);
		if (cos(angle * (M_PI / 180.f)) > 0)
		{
			glScalef(1.5f, 1.5f, 0);
		}
		else
		{
			glScalef(1.5f, -1.5f, 0);
		}
		glTranslatef(-m_Sprite->GetWidth() / 2, -m_Sprite->GetHeight() / 2, 0);
		m_Sprite->Draw();
	}
	glPopMatrix();
}

void Gun::Update(const float& elapsedSec)
{
	if (m_ShootTimer > 0)m_ShootTimer -= elapsedSec;
}

void Gun::Shoot(const Vector2f& dir)
{
	if (m_ShootTimer > 0)return;

	if (m_AmmoInClip == 0)Reload();
	m_AmmoInClip--;

	if (m_GunName[0] == 's' && m_GunName[1] == 'g')
	{
		for (size_t i = 0; i < 5; i++)
		{
			int spread{ 50 };
			Vector2f newDir{dir.x + float(rand() % spread - spread /2) / 100.f, dir.y + float(rand() % spread - spread / 2) / 100.f };
			g_BulletManager->ShootBullet(g_Player->GetPosition(), newDir, m_Att, false);
		}
	}
	else
	{
		g_BulletManager->ShootBullet(g_Player->GetPosition(), dir, m_Att, false);
	}
	g_SoundManager->PlaySound("PlayerShoot.wav");
	m_ShootTimer = m_ShootSpeed;
}

void Gun::Reload()
{
	if (m_AmmoInClip == m_MaxAmmo || m_Bullets == 0) return;

	int tempAmountInClip{ m_AmmoInClip };

	m_AmmoInClip += m_Bullets;
	if (m_AmmoInClip > m_MaxAmmo) m_AmmoInClip = m_MaxAmmo;
	int DeltaAmmo{ m_AmmoInClip - tempAmountInClip};
	m_Bullets -= DeltaAmmo;
}
