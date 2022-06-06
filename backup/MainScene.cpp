#include "pch.h"
#include "MainScene.h"
#include "Image.h"
#include "GameVariables.h"
#include "Tile.h"
#include <fstream>
#include "dirent.h"
#include "utils.h"
#include "ColourToObject.h"
#include "RoomHandler.h"

using namespace utils;

MainScene::MainScene():
	Scene(1)
{
	RoomHandler* room{ MakeRoom(0, Point2i(0,0)) };
	Point2f playerStartPosition{ 128, 128 };

	if (g_Player == nullptr) {
		g_Player = new Player{ playerStartPosition, &m_Window };
		g_Player->SetHp(6);
		g_Player->SetRoom(room);
	}
	g_BulletManager = new BulletManager();

	LoadScene();
}

MainScene::~MainScene() {
	delete g_Player;
	delete m_TransparentBall;
	delete m_TilesTexture;
	delete m_HeartTexture;
	delete m_AmmoTexture;
	delete m_TutorialTexture;
	
	for (auto& t : m_RoomHandlers) 
	{
		delete t;
	}
	for (auto& t : g_AllGameObjects)
	{
		delete t;
	}
	for (std::map<std::string, Texture*>::iterator itr = m_AllObjectTextures.begin(); itr != m_AllObjectTextures.end(); itr++)
	{
		delete itr->second;
	}
}

void MainScene::Start(const Window& w) {
	m_Window = w;
}

void MainScene::Draw() const{
	glPushMatrix();
	{
		//camera
		glTranslatef(Camera::Get().GetPos().x, Camera::Get().GetPos().y, 0);
		glScalef(Camera::Get().GetZoom(), Camera::Get().GetZoom(), 0);

		//content draw
		DrawLevel();
		g_Player->Draw();
		g_BulletManager->DrawBullets();
	}
	glPopMatrix();

	//ui and camera effects
	m_TransparentBall->Draw();
	g_UITextWriter->QueText(std::to_string(int(GetAvarageFps())) + " fps", Rectf(0,0,1.f,1.f));
	DrawHearts();
	DrawAmmo();
	g_UITextWriter->DrawAllText();
}

void MainScene::Update(const float& elapsedSec) 
{
	//std::cout << g_Player->GetRoom()->GetRoomIdx() << '\n';

	m_AllTicks.push_front(elapsedSec);
	if (m_AllTicks.size() > 100)
	{
		m_AllTicks.pop_back();
	}

	//input
	g_Player->Update(elapsedSec);

	//update camera
	Point2f playerPos{ g_Player->GetPosition()};
	Vector2f NewCameraPos{ (-playerPos.x) * Camera::Get().GetZoom() + m_Window.width / 2,(-playerPos.y) * Camera::Get().GetZoom() + m_Window.height / 2 };
	Camera::Get().SetPos(NewCameraPos);

	//update bullets
	g_BulletManager->UpdateBullets(elapsedSec);

	//update roomHandlers
	for (RoomHandler* room : m_RoomHandlers)
	{
		room->Update(elapsedSec); 
	}
}

void MainScene::DrawLevel() const
{
	for (RoomHandler* r : m_RoomHandlers)
	{
		r->DrawRoom();
	}
	m_TutorialTexture->Draw(Point2f(176,230));

	for (auto const& o : g_AllGameObjects)
	{
		if(o->IsActive())o->Draw();
	}
	
}

void MainScene::LoadScene() {
	InitializeGameAssets();
	MakeRoom(1, Point2i(7,14));
	MakeRoom(2, Point2i(33,14));
	MakeRoom(3, Point2i(42,-3));
	MakeRoom(4, Point2i(22,34));
	MakeRoom(5, Point2i(28,59));
	GetLevelTiles();
}	

void MainScene::LoadObject(const int& ObjectId, RoomHandler* ObjectRoom, const Point2i& Position)
{
	/* --loading of objects--
	* Room has enter door and end door.
	* if you kill all the enemys in the rooms the doors open
	*/

	//get the level image data into the stack
	//loop over to see if data matches with the table
	GameObject* newObj{ new GameObject() };
	//lookup in table
	bool isStatic{ GetGameObject(&newObj, ObjectId) };

	if (newObj->name != "empty") //auto name is "empty"
	{
		g_AllGameObjects.push_back(newObj);
		newObj->AddTexture(m_AllObjectTextures.at(newObj->name.c_str()));
		newObj->SetPosition(Point2f((float)Position.x * utils::g_SizeTiles, (float)Position.y * utils::g_SizeTiles));
		if (!isStatic) 
		{
			ObjectRoom->AddObject(newObj);
			newObj->SetRoom(ObjectRoom);

			if (newObj->GetType() != Type::Interactable)
			{
				g_AllBulletCollisionObjects.push_back(newObj);
			}
		}
	}
	else 
	{
		delete newObj;
	}
}

void MainScene::GetLevelTiles() {
	/* --Loading tiles--
	* Using colorchannels to get a certain tile from the assetsheet
	* r channel dictates type of tile (wall collsion, gameobject, ect.)
	* g channel always the underlaying tile
	* b channel id for the right type:
	*		-tile system works with coordinates on a sheet
	*		-GameObjects work with id and lookup table
	*/
	for (size_t roomNmr = 0; roomNmr < m_RoomHandlers.size(); roomNmr++)
	{
		RoomHandler* room{ m_RoomHandlers[roomNmr] };
		Image* roomImage{ room->GetImage() };
		Point2i roomPosition{ room->GetPosition()  };

		for (int x = 0; x < roomImage->w; x++)
		{
			for (int y = 0; y < roomImage->h; y++)
			{
				int idx{(y * roomImage->w + x) * 3};
				int TileType{ (int)roomImage->data[idx + 0] };//r
				int ColorValue{ (int)roomImage->data[idx + 1] };//g

				//layer 1 (green)
				int XTile{ ColorValue % 10 };
				int YTile{ ColorValue / 10 };
				Point2f Position{ (float)roomPosition.x + x, (float)roomPosition.y + (roomImage->h - y)};

				if (ColorValue != 0)
				{
					room->SetTileL1(new Tile(m_TilesTexture, Position, Point2i(XTile, YTile)));

					if (TileType & 32)
					{
						//layer 2 (blue)
						ColorValue = (int)roomImage->data[idx + 2];//b
						XTile = ColorValue % 10;
						YTile = ColorValue / 10;
					}
					else //empty tile
					{
						XTile = 0; //0
						YTile = 6; //6
					}

					room->SetTileL2(new Tile(m_TilesTexture, Position, Point2i(XTile, YTile)));
				}
				if (TileType & 128) LoadObject((int)roomImage->data[idx + 2], m_RoomHandlers[roomNmr], Point2i((int)Position.x, (int)Position.y));
			}
		}
	}
}

void MainScene::InitializeGameAssets() {

	//objects
	struct dirent* entry;
	std::string ObjectAssetPath{ "resources/objects/" };

	DIR* dir = opendir(ObjectAssetPath.c_str());
	
	if (dir == NULL) {
		return;
	}
	
	while ((entry = readdir(dir)) != NULL)
	{
		Texture* t = new Texture{ ObjectAssetPath + entry->d_name };
		m_AllObjectTextures.insert(std::pair<std::string, Texture*>(entry->d_name, t));
	}
	closedir(dir);
}

RoomHandler* MainScene::MakeRoom(int index, Point2i indexPosition)
{
	bool exist{ false };
	for (auto& r : m_RoomHandlers) 
	{
		if (indexPosition == r->GetPosition())return r;
	}

	std::string RoomPath{ "level/R" + std::to_string(index)};
	RoomPath += ".bmp";
	Image* myRoomImage{ new Image(RoomPath.c_str()) };
	RoomHandler* room{ new RoomHandler(m_RoomHandlers.size()) };

	room->SetImage(myRoomImage);
	room->SetPosition(indexPosition);
	m_RoomHandlers.push_back(room);
	return room;
}

void MainScene::DrawHearts() const
{
	float heartTextureWidth{ m_HeartTexture->GetWidth() / 3.f };
	int playerHP{ g_Player->GetHP() };

	Rectf srcHeart1{ 0,m_HeartTexture->GetHeight(),heartTextureWidth,m_HeartTexture->GetHeight() };
	Rectf dst{ 20, m_Window.height - m_HeartTexture->GetHeight() - 50, heartTextureWidth * 4, m_HeartTexture->GetHeight() * 4 };

	for (int i{}; i < g_Player->GetMaxHp(); i += 2)
	{
		if (playerHP <= i) srcHeart1.left += heartTextureWidth * 2;
		if (playerHP == i + 1) srcHeart1.left += heartTextureWidth;
		m_HeartTexture->Draw(dst, srcHeart1);
		srcHeart1.left = 0;
		dst.left += heartTextureWidth * 4;
	}
}

void MainScene::DrawAmmo() const
{
	const int margin{ 20 };
	const int Xposition{ (int)m_Window.width - (int)m_AmmoTexture->GetWidth() - margin * 2 };

	//bullets textures
	Rectf dest{(float)Xposition, 0, m_AmmoTexture->GetWidth() / 2 * 4, m_AmmoTexture->GetHeight() * 4};
	Rectf scr{ 0, m_AmmoTexture->GetHeight(), m_AmmoTexture->GetWidth() / 2, m_AmmoTexture->GetHeight() };
	int bulletsInClip{ g_Player->GetGun()->GetAmmoInClip() };
	int maxBullets{ g_Player->GetGun()->GetMaxAmmo() };

	for (int i{1}; i <= maxBullets; i++)
	{
		if (i <= bulletsInClip)
		{
			scr.left = 0;
		}
		else
		{
			scr.left = m_AmmoTexture->GetWidth() / 2;
		}
		dest.bottom = i * (margin/2 + m_AmmoTexture->GetHeight() * 2);
		m_AmmoTexture->Draw(dest,scr);
	}
	Texture* gunIcon{ g_Player->GetGun()->GetIcon() };
	Point2f gunSize{ gunIcon->GetWidth() * 8 ,gunIcon->GetHeight() * 8 };

	utils::SetColor(Color4f(0.6f,0.6f,0.6f,1));

	//gun counter
	for (int i = 1; i < g_Player->GetAmountOfGuns(); i++)
	{
		DrawRect(Rectf(Xposition - gunSize.x - margin * 3 - margin/2 * i, margin * 2 + 1, gunSize.x + margin * 2, gunSize.y + margin * 2 - 2), 3);
	}

	//icon
	utils::FillRect(Rectf(Xposition - gunSize.x -margin * 3, margin*2, gunSize.x + margin * 2, gunSize.y + margin * 2));
	gunIcon->Draw(Rectf(Xposition - gunSize.x- margin*2, margin*3, gunSize.x, gunSize.y));
	g_UITextWriter->QueText(std::to_string(g_Player->GetGun()->GetBullets())+"/"+ std::to_string(g_Player->GetGun()->GetMaxBullets()),Rectf((float)Xposition - 200, margin*4+gunSize.y,1.4f,1.4f));

}

float MainScene::GetAvarageFps() const
{
	float all{};
	for (float f : m_AllTicks)
	{
		all += f;
	}
	return 1/ (all / m_AllTicks.size());
}