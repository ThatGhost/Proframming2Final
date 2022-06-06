#include "pch.h"
#include "DungeonDoor.h"
#include "Texture.h"
#include "Image.h"

DungeonDoor::DungeonDoor(std::string name) :
	GameObject(name, Type::Door),
	m_Unlocked{ false }
{
	m_HP = INT16_MAX;
}

void DungeonDoor::Draw() const
{
	Rectf src{ m_Sprite->GetWidth() / 2, m_Sprite->GetHeight(), m_Sprite->GetWidth() / 2, m_Sprite->GetHeight() };
	if (!m_Unlocked)src.left = 0;

	m_Sprite->Draw(m_Position,src);
}

void DungeonDoor::UnlockDoor(const bool& toggle)
{
	m_Unlocked = toggle;
}