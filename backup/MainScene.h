#pragma once
#include "Texture.h"
#include "BasicBehaviour.h"
#include "Scene.h"
#include <vector>
#include <map>
#include <list>

class Tile;
class Player;
struct Image;
class Camera;
class RoomHandler;
class Game;

class MainScene : public Scene
{
public:
	MainScene();
	virtual void Start(const Window& window) override;
	virtual void Draw() const override;
	virtual void Update(const float& elapsedSec) override;

	virtual ~MainScene() override;
	MainScene(const MainScene& other) = delete;
	MainScene& operator=(const MainScene& other) = delete;
	MainScene(MainScene&& other) = delete;
	MainScene& operator=(MainScene&& other) = delete;
private:
	//general
	Window m_Window;

	//collections
	std::vector<RoomHandler*> m_RoomHandlers;
	std::map<std::string, Texture*> m_AllObjectTextures;
	std::list<float> m_AllTicks{};

	//textures
	Texture* m_TransparentBall{new Texture("resources/viewball.png")};
	Texture* m_TilesTexture{new Texture("resources/Dungeon_Tileset.png")};
	Texture* m_HeartTexture{new Texture("resources/Heart.png")};
	Texture* m_AmmoTexture{new Texture("resources/AmmoSprites.png")};
	Texture* m_TutorialTexture{new Texture("resources/Tutorial.png")};

	//functions
	void InitializeGameAssets();
	void DrawLevel() const;
	void LoadScene();
	void LoadObject(const int& ObjectId, RoomHandler* RoomImage, const Point2i& Position);
	void GetLevelTiles();
	void DrawHearts() const;
	void DrawAmmo() const;

	//return type functions
	RoomHandler* MakeRoom(int index, Point2i indexPosition);
	float GetAvarageFps() const;
};

