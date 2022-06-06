#include "pch.h"
#include "Tile.h"
#include "Texture.h"
#include "Player.h"

extern Player* g_Player;

Tile::Tile(Texture* t, const Point2f& Pos, const Point2i dest) :
	m_Pos{ Pos }, 
	m_Sprite{ t }, 
	m_SrcPos{dest}
{}

void Tile::Draw() const {
	const int MaxDist{ 500 };

	Point2f dest{ m_Pos.x * m_SizeTile, m_Pos.y * m_SizeTile };
	Point2f pPos{ g_Player->GetPosition() };
	if (sqrt((dest.x - pPos.x) * (dest.x - pPos.x) + (dest.y - pPos.y) * (dest.y - pPos.y)) > MaxDist) return;

	Rectf src{ (float)m_SrcPos.x * m_SizeTile, (float)(m_SrcPos.y) * m_SizeTile, (float)m_SizeTile, (float)m_SizeTile};
	m_Sprite->Draw(dest,src);
}
