#pragma once
#include "Camera.h"
#include "BulletManager.h"
#include "Object.h"
#include "Player.h"
#include "UITextWriter.h"
#include "SoundManager.h"
#include "Game.h"
#include <vector>

#ifndef GAMEVARIABLES_H
#define GAMEVARIABLES_H

extern InputS Input;
extern std::vector<GameObject*> g_AllGameObjects;
extern std::vector<GameObject*> g_AllBulletCollisionObjects;
extern Player* g_Player;
extern BulletManager* g_BulletManager;
extern UITextWriter* g_UITextWriter;
extern SoundManager* g_SoundManager;
extern Point2f g_WindowSize;
extern Game* g_pGame;


#endif