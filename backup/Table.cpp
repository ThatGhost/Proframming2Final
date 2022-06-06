#include "pch.h"
#include "Table.h"
#include "GameVariables.h"
#include "utils.h"

Table::Table() :
	GameObject("Table.png", Type::Interactable)
{
	m_HP = INT16_MAX;
}

void Table::Interact() 
{
	g_AllBulletCollisionObjects.push_back(this);
	m_Trown = true;
}

void Table::Draw() const 
{
	if (m_MyRoom != g_Player->GetRoom()) return;
	Rectf src{ m_Sprite->GetWidth() / 2, m_Sprite->GetHeight(), m_Sprite->GetWidth() / 2, m_Sprite->GetHeight() };
	if (!m_Trown)src.left = 0;

	m_Sprite->Draw(m_Position, src);
}
