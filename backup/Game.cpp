#include "pch.h"
#include <iostream>
#include <cassert>
#include <sstream>
#include "Game.h"
#include "GameVariables.h"
#include "utils.h"
#include "Vector2f.h"
#include "Camera.h"
#include "UITextWriter.h"
#include "MenuScene.h"
#include "MainScene.h"

Player* g_Player{ nullptr };
InputS Input;
std::vector<GameObject*> g_AllGameObjects;
std::vector<GameObject*> g_AllBulletCollisionObjects;
BulletManager* g_BulletManager{nullptr};
UITextWriter* g_UITextWriter{nullptr};
SoundManager* g_SoundManager{nullptr};
Point2f g_WindowSize{ 1180.f ,700.f };
Game* g_pGame{nullptr};

Game::Game( const Window& window )
	:m_Window{ window }
{	 
	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	g_pGame = this;

	m_Scenes.push_back(new MenuScene());
	m_Scenes.push_back(new MainScene());

	m_Scenes[1]->Start(m_Window);
	g_UITextWriter = new UITextWriter();
	g_SoundManager = new SoundManager();
}

void Game::Cleanup( )
{
	delete g_BulletManager;
	delete g_UITextWriter;
	delete g_SoundManager;
	for (Scene* s : m_Scenes)
	{
		delete s;
	}
}

void Game::Update( float elapsedSec )
{
	m_Scenes[m_CurrentSceneIdx]->Update(elapsedSec);
}

void Game::Draw( ) const
{
	ClearBackground();
	m_Scenes[m_CurrentSceneIdx]->Draw();
}

void Game::ClearBackground() const
{
	glClearColor(47/ 255.f, 47 / 255.f, 46 / 255.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Game::GoBackToMain()
{
	delete m_Scenes[1];
	delete g_BulletManager;
	delete g_Player;
	g_AllGameObjects.clear();
	g_AllBulletCollisionObjects.clear();
	m_CurrentSceneIdx = 0;
	m_Scenes[1] = new MainScene();
}






