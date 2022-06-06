#pragma once
#include "Scene.h"
#include "Texture.h"
#include <vector>

class Game;
class Button;

struct RenderObject {
	Point2f pos;
	Texture* texture{nullptr};
	bool operator<(const RenderObject& rhs) { return this->pos.y < rhs.pos.y; }
};

class MenuScene : public Scene
{
public:
	MenuScene();
	virtual ~MenuScene() override;
	virtual void Draw() const override;
	virtual void Update(const float& elapsedSec) override;
private:
	Button* m_PlayButton{nullptr};
	Button* m_OptionsButton{ nullptr };
	Button* m_ExitButton{ nullptr };
	const int m_Margin{ 20 };

	void DrawUI() const;
	void DrawSim() const;

	Texture* m_pPlayer{new Texture("resources/player.png")};
	Point2f m_PlayerPos{};

	Texture* m_pPlayerGun{new Texture("resources/guns/mg5000.png")};
	Texture* m_pTile{new Texture("resources/basetile.png")};
	Texture* m_pWallTile{new Texture("resources/WallTile.png")};
	std::vector<Texture*> m_EnemyTextures{};
	std::vector<RenderObject> m_Enemys{};

	//animations
	const int m_MaxFrame{4};
	int m_Frame{};
	float m_AniTimer{};
	float m_AniTime{0.25f};
	float m_ScaleSprite{3.f};

	bool m_GoingLeft{};
};

