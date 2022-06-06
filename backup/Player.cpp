#include "pch.h"
#include "GameVariables.h"
#include "utils.h"
#include "Texture.h"
#include "Image.h"
#include "Gun.h"
#include <map>

using namespace utils;

Player::Player(const Point2f& pos, Window* Window)
	:GameObject("Player", pos, Type::Player), 
	 m_Window{ Window} 
{
	m_Sprite = new Texture("Resources/Player.png");
	m_Guns.push_back(new Gun("sg1"));
}

Player::~Player() {
	for (Gun* e : m_Guns)
	{
		delete e;
	}
	delete m_Sprite;
}

void Player::Draw() const {

	//animation calculations
	Point2f sizeOfFrame{ m_Sprite->GetWidth() / (m_NrOfFrames * 2) ,m_Sprite->GetHeight() / m_NrOfRows };
	int xOffset{};
	if (m_ActionState == ActionState::moving)
	{
		xOffset = 4 * (int)sizeOfFrame.x;
	}

	//draw character
	Rectf src{ (float)xOffset + m_Frame * sizeOfFrame.x, sizeOfFrame.y, sizeOfFrame.x, sizeOfFrame.y };
	if (!m_Alive) 
	{
		src.bottom += sizeOfFrame.y*2;
		glPushMatrix();
		{
			glTranslatef(m_Position.x, m_Position.y, 0);
			glScalef(1.5f, 1.5f, 0);
			glTranslatef(-sizeOfFrame.x / 2, -sizeOfFrame.y / 2, 0);
			m_Sprite->Draw(Point2f(0, 0), src);
			SetColor(Color4f(0,0,0,1));
			FillEllipse(sizeOfFrame.x / 2, sizeOfFrame.y / 2, m_DeathCirlceSize, m_DeathCirlceSize);
		}
		glPopMatrix();
	}
	else //when alive
	{
		glPushMatrix();
		{
			glTranslatef(m_Position.x, m_Position.y, 0);
			//look to left when going left
			int sign{ m_RunningRight ? 1 : -1};
			glScalef(sign*1.5f, 1.5f, 0);

			if (m_InvincTimer > 0 && m_Damaged)
			{
				if (int(m_InvincTimer * 10) % 3 == 0)src.bottom += sizeOfFrame.y;
			}
			else if (m_InvincTimer > 0)
			{
				GLfloat angle{m_InvincTimer/m_InvincTime * 360.f};
				glRotatef(angle,0,0,1);
			}
			glTranslatef(-sizeOfFrame.x / 2, -sizeOfFrame.y / 2, 0);
			m_Sprite->Draw(Point2f(0, 0), src);

		}
		glPopMatrix();

		//gun
		Point2f gunPos{ m_Position.x, m_Position.y + 3 };
		//if (!m_RunningRight) gunPos.x -= 25;
		m_Guns[m_CurrentGun]->Draw(GetGunAngle(), gunPos);

		Point2f MousePoint{ ScreenToWorldPoint(Point2f( Input.mousePos.x,g_WindowSize.y - Input.mousePos.y))};

		glLineStipple(6, 0xAAAA);
		glEnable(GL_LINE_STIPPLE);
		DrawLine(MousePoint, m_Position,4);
		DrawEllipse(MousePoint, 5,5,5);
		glEnable(GL_LINE);
	}
}

void Player::Update(const float& elapsedSec) 
{
	//animations
	if (m_AnimTimer > 0) m_AnimTimer -= elapsedSec;
	else
	{
		m_Frame++;
		if (m_Frame == m_NrOfFrames)m_Frame = 0;
		m_AnimTimer = 1.0f / m_FramesPerSec;
	}

	if (!m_Alive)
	{
		m_EndTimer -= elapsedSec;
		if (m_EndTimer <= 0)
		{
			g_pGame->GoBackToMain();
		}
		m_DeathCirlceSize += elapsedSec * 50;
		g_UITextWriter->QueText("You Died", Rectf(g_WindowSize.x / 2 - 200, g_WindowSize.y / 2 + 100, 4, 4));
		if (m_DeathCirlceSize >= 200)g_pGame->SetCurrentSceneIdx(0);
		return;
	}

	//invincible timer
	if (m_InvincTimer > 0)
	{
		m_InvincTimer -= elapsedSec;
	}
	else
	{
		m_Damaged = false;
	}

	//interact
	if (Input.keyUp == SDLK_e || Input.keyUp == SDLK_f)
	{
		Point2f playerMid{ m_Position.x + m_Sprite->GetWidth()/ m_NrOfFrames /2,m_Position.y + m_Sprite->GetHeight() / m_NrOfRows / 2 };
		m_MyRoom->InteractWithObjects(Point2f(playerMid));
	}

	//gun
	float gunAngle{ GetGunAngleRad()};

	m_Guns[m_CurrentGun]->Update(elapsedSec);

	//input
	if (Input.ScrollWheel > 0) {
		m_CurrentGun++;
		if (m_CurrentGun == m_Guns.size())
			m_CurrentGun = 0;
	}
	else if (Input.ScrollWheel < 0)
	{
		m_CurrentGun--;
		if (m_CurrentGun < 0)
			m_CurrentGun = m_Guns.size() - 1;
	}

	if (Input.ClickDown == ClickState::MB1) {
		m_Guns[m_CurrentGun]->Shoot(Vector2f(cos(gunAngle), sin(gunAngle)));
	}

	if (Input.keyDown == SDLK_r)
	{
		m_Guns[m_CurrentGun]->Reload();
		m_Guns[m_CurrentGun]->Reload();
	}

	// movement
	const Uint8* state{SDL_GetKeyboardState(NULL)};
	Vector2f dir{};

	if (state[SDL_SCANCODE_W]) {
		dir.y = m_Speed * elapsedSec;
	}
	if (state[SDL_SCANCODE_S]) {
		dir.y = -m_Speed * elapsedSec;
	}
	if (state[SDL_SCANCODE_A]) {
		dir.x = -m_Speed * elapsedSec;
	}
	if (state[SDL_SCANCODE_D]) {
		dir.x = +m_Speed * elapsedSec;
	}

	if (dir.x == 0 && dir.y == 0)m_ActionState = ActionState::idle;
	else m_ActionState = ActionState::moving;

	if (dir.x < 0) m_RunningRight = false;
	else if (dir.x > 0)m_RunningRight = true;

	dir = dir.Normalized();

	Rectf playerCollider{ GetCollider()};
	playerCollider.left += dir.x * 2;
	playerCollider.bottom += dir.y * 2;

	if (Input.keyUp == SDLK_LSHIFT && m_DashTimer <= 0
	 && (dir.x != 0 || dir.y != 0))
	{
		m_DashTimer = m_DashTime;
		m_Speed = m_DashSpeed;
		m_InvincTimer = m_DashTime;
	}

	if (m_DashTimer > 0) m_DashTimer -= elapsedSec;
	else
	{
		m_Speed = m_NormalSpeed;
	}

	if (!IsCollidingWithWall(playerCollider)) { //!!!!!!!!!
		m_Position += Point2f(dir.x * m_Speed * elapsedSec, dir.y * m_Speed * elapsedSec);
	}
	
}

Rectf Player::GetCollider() const 
{
	return Rectf ( m_Position.x - (m_Sprite->GetWidth() / (m_NrOfFrames * 2) / 2),
				   m_Position.y - (m_Sprite->GetHeight() / m_NrOfRows / 2) - 5,
				   m_Sprite->GetWidth() / (m_NrOfFrames * 2), 
				   m_Sprite->GetHeight() / m_NrOfRows);
}

Point2f Player::GetPosition()const  {
	return m_Position;
}

float Player::GetGunAngle() const
{
	Point2f mousePos{ Input.mousePos.x, m_Window->height - Input.mousePos.y };
	Point2f mousePosInWorld = ScreenToWorldPoint(mousePos);

	float deltaY{ mousePosInWorld.y - m_Position.y };
	float deltaX{ mousePosInWorld.x - m_Position.x };

	float angle{ atan2f(deltaY, deltaX) };
	return angle *= (float)(180 / M_PI);
}

float Player::GetGunAngleRad() const
{
	Point2f mousePos{ Input.mousePos.x, m_Window->height - Input.mousePos.y };
	Point2f mousePosInWorld = ScreenToWorldPoint(mousePos);

	float deltaY{ mousePosInWorld.y - m_Position.y };
	float deltaX{ mousePosInWorld.x - m_Position.x };

	return atan2f(deltaY, deltaX);
}

void Player::Damage(const int& damage)
{
	if (m_InvincTimer <= 0 && m_Alive)
	{
		m_Damaged = true;
		m_InvincTimer = m_InvincTime;
		m_HP--;
		if (m_HP <= 0)Die();
	}
}

void Player::Die()
{
	m_Alive = false;
}

bool Player::AddGun(const std::string& gunName)
{
	bool canAd{true};
	for (Gun* g : m_Guns)
	{
		if (g->name == gunName)canAd = false;
	}
	if(canAd)m_Guns.push_back(new Gun(gunName));
	return canAd;
}