#include "pch.h"
#include "RoomHandler.h"
#include "GameVariables.h"
#include "utils.h"
#include "Object.h"
#include "DungeonDoor.h"
#include "Enemy.h"
#include "Image.h"
#include "Tile.h"

RoomHandler::RoomHandler(const int& index):
	m_RoomIndex{index}
{}

void RoomHandler::AddObject(GameObject* obj)
{
	if (obj->name == "Door.png" || obj->name == "DoorV.png")
	{
		DungeonDoor* door = static_cast<DungeonDoor*>(obj);
		door->UnlockDoor(true);
		m_Doors.push_back(door);
	}
	else {
		m_Objects.push_back(obj);
	}
}

void RoomHandler::DrawRoom() const
{
	for (Tile* t : m_RoomTilesL1)
	{
		t->Draw();
	}
	for (Tile* t : m_RoomTilesL2)
	{
		t->Draw();
	}
}

void RoomHandler::Update(const float& elapsedSec)
{
	Point2f playerPos{ g_Player->GetPosition() };
	Rectf MyArea{ ((float)m_Position.x + 2) * utils::g_SizeTiles, ((float)m_Position.y + 2) * utils::g_SizeTiles,
		((float)m_RoomImage->w - 1) * utils::g_SizeTiles, ((float)m_RoomImage->h - 1)* utils::g_SizeTiles };
	if (utils::IsPointInRect(playerPos, MyArea))
	{
		if (!m_Trigger)
		{
			g_Player->SetRoom(this);
			m_DelayTimer = 1.0f;
			m_Trigger = true;
		}
		else {
			if (m_Doors[0]->IsUnlocked())
			{
				if (m_DelayTimer <= 0) {
					for (DungeonDoor* door : m_Doors)door->UnlockDoor(false);
				}
				else m_DelayTimer -= elapsedSec;
			}
		}

		for (GameObject* b : m_Objects)
		{
			if (b->IsActive())
			{
				b->Update(elapsedSec);
			}
		}

		if (IsEmpty())
		{
			for(DungeonDoor* door: m_Doors)door->UnlockDoor(true);
		}
	}
	else if(m_Trigger)
	{
		m_Trigger = false;
	}
}

bool RoomHandler::IsEmpty() const
{
	bool empty = true;
	for (auto const& a : m_Objects) {
		if (a->GetType() == Type::Enemy && a->IsActive())empty = false;
	}
	return empty;
}

RoomHandler::~RoomHandler()
{
	delete m_RoomImage;
	for (Tile* t : m_RoomTilesL1)
	{
		delete t;
	}
	for (Tile* t : m_RoomTilesL2)
	{
		delete t;
	}
}

void RoomHandler::InteractWithObjects(const Point2f& PlayerPos)
{
	for (auto const& e : m_Objects)
	{
		if (e->GetType() == Type::Interactable)
		{
			Rectf extendedCollider{ e->GetCollider() };
			extendedCollider.left -= 20;
			extendedCollider.bottom -= 20;
			extendedCollider.width += 40;
			extendedCollider.height += 40;

			if (utils::IsPointInRect(PlayerPos, extendedCollider))
			{
				e->Interact();
			}
		}
	}
}