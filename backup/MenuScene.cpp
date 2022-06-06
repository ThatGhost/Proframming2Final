#include "pch.h"
#include "MenuScene.h"
#include "Button.h"
#include "UITextWriter.h"
#include "Game.h"
#include <algorithm>

extern Point2f g_WindowSize;
extern Game* g_pGame;
extern UITextWriter* g_UITextWriter;

MenuScene::MenuScene() :
	Scene(0), m_PlayerPos{0,g_WindowSize.y / 2}
{
	const Point2f startPos{-500,g_WindowSize.y / 2 };

	m_PlayButton = new Button("play.png",Point2f(g_WindowSize.x/2,260));
	m_OptionsButton = new Button("options.png", Point2f(g_WindowSize.x / 2, 250 + (float)m_Margin));
	m_ExitButton = new Button("exit.png", Point2f(g_WindowSize.x / 2, 150));
	m_EnemyTextures.push_back(new Texture("resources/objects/E_Fiery.png"));
	m_EnemyTextures.push_back(new Texture("resources/objects/E_Grum.png"));
	m_EnemyTextures.push_back(new Texture("resources/objects/E_Kato.png"));
	m_EnemyTextures.push_back(new Texture("resources/objects/E_Mini.png"));
	m_EnemyTextures.push_back(new Texture("resources/objects/E_Pum.png"));
	m_EnemyTextures.push_back(new Texture("resources/objects/E_Richard.png"));
	m_EnemyTextures.push_back(new Texture("resources/objects/E_Tato.png"));

	for (size_t i = 0; i < 20; i++)
	{
		RenderObject obj;

		float randomX{rand() % 3500 / 10.f - 150};
		float randomY{rand() % 3500 / 10.f - 150};
		obj.pos = Point2f(startPos.x + randomX, startPos.y + randomY);
		obj.texture = m_EnemyTextures[rand() % m_EnemyTextures.size()];
		m_Enemys.push_back(obj);
	}
	RenderObject obj;
	std::sort(m_Enemys.begin(), m_Enemys.end());
	obj.pos = Point2f(startPos.x - 350, startPos.y);
	obj.texture = m_EnemyTextures[m_EnemyTextures.size()-1];
	m_Enemys.push_back(obj);
}

MenuScene::~MenuScene()
{
	delete m_pPlayer;
	delete m_ExitButton;
	delete m_PlayButton;
	delete m_OptionsButton;
	delete m_pPlayerGun;
	delete m_pTile;
	delete m_pWallTile;
	for (Texture* t : m_EnemyTextures)
	{
		delete t;
	}
}

void MenuScene::Draw() const
{
	DrawSim();

	DrawUI();
}

void MenuScene::Update(const float& elapsedSec)
{
	m_PlayButton->Update();
	m_OptionsButton->Update();
	m_ExitButton->Update();

	if (m_PlayButton->IsClicked())
	{
		g_pGame->SetCurrentSceneIdx(1);
	}
	else if (m_OptionsButton->IsClicked())
	{
		//std::cout << "to settings\n";
		//g_pGame->SetCurrentSceneIdx(1);
	}
	else if (m_ExitButton->IsClicked())
	{
		g_pGame->QuitGame();
	}

	//update bg
	const float speed{150};
	if (!m_GoingLeft)
	{
		m_PlayerPos.x += elapsedSec * speed;

		for (size_t i = 0; i < m_Enemys.size(); i++)
		{
			m_Enemys[i].pos.x += elapsedSec * speed;
		}
	}
	else
	{
		m_PlayerPos.x -= elapsedSec * (speed * 2);

		for (size_t i = 0; i < m_Enemys.size(); i++)
		{
			m_Enemys[i].pos.x -= elapsedSec * (speed * 2);
		}
	}

	if (m_AniTimer <= 0)
	{
		m_Frame++;
		if (m_Frame == m_MaxFrame) m_Frame = 0;
		m_AniTimer = m_AniTime;
	}
	else m_AniTimer -= elapsedSec;

	if (m_Enemys[m_Enemys.size() - 1].pos.x > g_WindowSize.x + 100)
	{
		m_GoingLeft = true;
		m_AniTime = 0.1f;
	}
	if (m_PlayerPos.x <= -100)
	{
		m_GoingLeft = false;
		m_AniTime = 0.25f;
	}
}

void MenuScene::DrawUI() const
{
	m_PlayButton->Draw();
	//m_OptionsButton->Draw();
	m_ExitButton->Draw();
	g_UITextWriter->QueText("GUN FUN", Rectf(g_WindowSize.x / 2 - 300, 500, 4, 4));
	g_UITextWriter->DrawAllText();
}

void MenuScene::DrawSim() const
{
	for (int y = 0; y < (g_WindowSize.y / 64) + 1; y++)
	{
		for (int x = 0; x < g_WindowSize.x / 64; x++)
		{
			glPushMatrix();
			{
				glTranslatef((GLfloat) x * 64,(GLfloat) y * 64, 0);
				glScalef(2, 2, 1);
				glTranslatef(-16, -16, 0);
				m_pTile->Draw();
			}
			glPopMatrix();
		}
	}
	for (int x = 0; x < g_WindowSize.x / 64; x++)
	{
		m_pWallTile->Draw(Point2f((float)x*64, (float)((g_WindowSize.y/64) - 1) * 64));
	}

	int scale{ m_GoingLeft ? -1 : 1 };

	//player
	Point2f sizeOfFrame{ m_pPlayer->GetWidth() / 8 ,m_pPlayer->GetHeight() / 3 };
	Rectf src{ sizeOfFrame.x * 4 + m_Frame * sizeOfFrame.x, sizeOfFrame.y, sizeOfFrame.x, sizeOfFrame.y };
	glPushMatrix();
	{
		glTranslatef(m_PlayerPos.x,m_PlayerPos.y,0);
		glScalef(scale * m_ScaleSprite, m_ScaleSprite, 1);
		glTranslatef(-sizeOfFrame.x/2,-sizeOfFrame.y/2,0);
		m_pPlayer->Draw(Point2f(), src);
		if (m_GoingLeft)
		{
			m_pPlayerGun->Draw(Point2f(0,5));
		}
	}
	glPopMatrix();

	//enemys
	for (int i = m_Enemys.size()-1; i >=0 ; i--)
	{
		Point2f sizeOfFrame{ m_Enemys[i].texture->GetWidth() / 8 ,m_Enemys[i].texture->GetHeight() };

		Rectf src{ sizeOfFrame.x * 4 + m_Frame * sizeOfFrame.x, sizeOfFrame.y, sizeOfFrame.x, sizeOfFrame.y };
		glPushMatrix();
		{
			glTranslatef(m_Enemys[i].pos.x, m_Enemys[i].pos.y, 0);
			glScalef(scale* m_ScaleSprite, m_ScaleSprite, 1);
			glTranslatef(-sizeOfFrame.x / 2, -sizeOfFrame.y / 2, 0);
			m_Enemys[i].texture->Draw(Point2f(), src);
		}
		glPopMatrix();

	}
}