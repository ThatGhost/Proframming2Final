#include "pch.h"
#include "Object.h"
#include "Texture.h"
#include "GameVariables.h"
#include "utils.h"
#include "DungeonDoor.h"
#include "Image.h"


void GameObject::Damage(const int& damage)
{
	m_HP -= damage;
	if (m_HP <= 0)Die();
}

bool GameObject::IsCollidingWithWall(const Rectf& collider)
{
	Point2f position{ collider.left - m_MyRoom->GetPosition().x * utils::g_SizeTiles,collider.bottom - m_MyRoom->GetPosition().y * utils::g_SizeTiles };
	Rectf newCollider{position.x,position.y,collider.width,collider.height};
	bool isColliding{ false };
	Image* bounds{ m_MyRoom->GetImage() };
	//checking in a 3x3 grind around me to check for walls (red colordata == 33)
	//if the "future collider" collides with any of the walls then you cant move in that dir
	for (int y = -1; y < 2; y++)
	{
		for (int x = -1; x < 2; x++)
		{
			Point2i currentPosIdx{ (int)(position.x / utils::g_SizeTiles + x), (int)(position.y / utils::g_SizeTiles + y) };
			Rectf square{ (float)currentPosIdx.x * utils::g_SizeTiles, (float)currentPosIdx.y * utils::g_SizeTiles, utils::g_SizeTiles, utils::g_SizeTiles };
			if (utils::IsOverlapping(square, newCollider)) {
				int data{ bounds->data[((bounds->h - currentPosIdx.y) * bounds->w + (currentPosIdx.x)) * 3] };
				if (data == 33 || data == 34) {//33 || 34
					isColliding = true;
				}
			}
		}
	}
	for (GameObject* g : g_AllGameObjects) {
		if (g->name == "Door.png" || g->name == "DoorV.png") {
			if (utils::IsOverlapping(g->GetCollider(), collider))
			{
				isColliding = !static_cast<DungeonDoor*>(g)->IsUnlocked();
			}
		}
		else if (g->name == "Table.png" || g->name == "TableV.png")
		{
			if (utils::IsOverlapping(g->GetCollider(), collider))
			{
				isColliding = true;
			}
		}
	}
	return isColliding;
}