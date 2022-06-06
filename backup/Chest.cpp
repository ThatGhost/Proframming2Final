#include "pch.h"
#include "Chest.h"
#include "utils.h"
#include "Gun.h"
#include "GameVariables.h"
#include <fstream>
#include <sstream>

Chest::Chest() :
	GameObject("Chest.png", Type::Interactable)
{
	m_HP = INT16_MAX;
}

void Chest::Interact()
{
	if (m_Open)return;

	m_Open = true;

	std::ifstream iStream{ "resources/guns/gunstats.txt" };
	std::string line{};
	int i{};
	bool foundGun{ false };

	if (iStream.is_open()) 
	{
		do {
			int randomLine{ rand() % 26 };
			while (std::getline(iStream, line))
			{
				if (i == randomLine) {
					std::istringstream iss(line);
					std::string nameGun;
					iss >> nameGun;
					foundGun = g_Player->AddGun(nameGun);
				}
				i++;
			}
		} while (!foundGun);
	}

	iStream.close();
	g_Player->AddHealth();
}

void Chest::Update(const float& elapsedSec)
{
	if (m_Open && m_PopUpTimer >= 0)m_PopUpTimer -= elapsedSec;
}

void Chest::Draw() const
{
	if (m_MyRoom != g_Player->GetRoom()) return;
	Rectf src{ m_Sprite->GetWidth() / 2, m_Sprite->GetHeight(), m_Sprite->GetWidth() / 2, m_Sprite->GetHeight() };
	if (!m_Open)src.left = 0;

	m_Sprite->Draw(m_Position, src);

	if (m_Open && m_PopUpTimer > 0)
	{
		g_UITextWriter->QueText("+1 GUN",Rectf(g_WindowSize.x/2 - 50, g_WindowSize.y / 2 - 70,1,1));
		g_UITextWriter->QueText("+1 HP",Rectf(g_WindowSize.x / 2 - 50, g_WindowSize.y / 2 - 90,1,1));
	}
}
