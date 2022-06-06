#pragma once
#include <vector>

struct Image;
class GameObject;
class DungeonDoor;
class Tile;

class RoomHandler
{
public:
	RoomHandler(const int& index);
	~RoomHandler();

	void Update(const float& elapsedSec);
	void DrawRoom() const;
	void AddObject(GameObject* obj);
	void SetPosition(Point2i pos) { m_Position = pos; }
	void SetImage(Image* image) { m_RoomImage = image; }
	void InteractWithObjects(const Point2f& PlayerPos);

	Point2i GetPosition() const { return m_Position; }
	int GetRoomIdx() const { return m_RoomIndex; }
	Image* GetImage() const { return m_RoomImage; }
	bool IsEmpty() const;

	Tile* GetTileL1(int idx) const { return m_RoomTilesL1[idx]; }
	void SetTileL1(Tile* tile) { m_RoomTilesL1.push_back(tile); }
	Tile* GetTileL2(int idx) const { return m_RoomTilesL2[idx]; }
	void SetTileL2(Tile* tile) { m_RoomTilesL2.push_back(tile); }
private:
	//general
	Point2i m_Position{};
	Image* m_RoomImage{ nullptr };
	int m_RoomIndex;

	//animations (door closing)
	bool m_Trigger{ false };
	float m_DelayTimer{};

	//collections
	std::vector<GameObject*> m_Objects{};
	std::vector<DungeonDoor*> m_Doors{};
	std::vector<Tile*> m_RoomTilesL1{};
	std::vector<Tile*> m_RoomTilesL2{};

};

