#include "pch.h"
#include "Boss.h"
#include "GameVariables.h"
#include "utils.h"

Boss::Boss() : GameObject("Boss.png", Type::Enemy)
{
	m_HP = m_ActualMaxHealth;
}

Boss::~Boss()
{
	delete m_Attack;
	delete m_Idle;
	delete m_Death;
}

void Boss::Draw() const
{

	int hp{ m_HP - (m_ActualMaxHealth - m_MaxHealth) };
	if (hp <= 0)
	{
		utils::SetColor(Color4f(0.9f,0.9f,0.9f,1));
		utils::FillEllipse(m_Position.x + 54,m_Position.y + 46, m_ElipseSize, m_ElipseSize);
	}
	if (!m_Won)
	{
		Rectf scr{ m_Frame * m_FrameSize, m_FrameSize, m_FrameSize, m_FrameSize };
		if (m_State == BossState::Attacking)m_Attack->Draw(m_Position, scr);
		if (m_State == BossState::Idle)m_Idle->Draw(m_Position, scr);
		if (m_State == BossState::Death)m_Death->Draw(m_Position, scr);
	}
}

void Boss::Update(const float& elapsedSec)
{
	if (m_MyRoom != g_Player->GetRoom())return;

	int hp{ m_HP - (m_ActualMaxHealth - m_MaxHealth) };
	if (hp <= 0)
	{
		m_State = BossState::Death;
		if (m_ElipseSize <= 600)m_ElipseSize += elapsedSec * m_ElipseSize * 3;
	}

	//animations
	if (m_AnimTimer >= 0)
	{
		if (!m_Won) m_AnimTimer -= elapsedSec;
	}
	else
	{
		m_AnimTimer = m_AnimationSpeed;
		m_Frame++;
		if (m_State == BossState::Attacking && m_Frame == m_AttackFrameAmount) { m_Frame = 0; m_State = BossState::Idle; m_AttackTimer = m_AttackTime; }
		if (m_State == BossState::Idle && m_Frame == m_IdleFrameAmount)m_Frame = 0;
	}

	//AI (state machine)
	switch (m_State)
	{
	case BossState::Idle:
		if (m_AttackTimer > 0)m_AttackTimer -= elapsedSec;
		else
		{
			m_State = BossState::Attacking;
		}

		break;
	case BossState::Attacking:
		if (m_Frame == 3 || m_Frame == 10) m_Attacked = false;
		if (!m_Attacked && (m_Frame == 2 || m_Frame == 9))Attack();

		break;
	case BossState::Death:
		if (m_Frame == m_DeathFrameAmount)
		{
			m_Won = true;
		}

		break;
	}

	if (m_Won)
	{
		m_EndTimer -= elapsedSec;
		if (m_EndTimer <= 0)
		{
			g_pGame->GoBackToMain();
		}
		g_UITextWriter->QueText("You won!", Rectf(g_WindowSize.x / 2 - 250, g_WindowSize.y / 2 + 100, 4, 4));
	}
}

void Boss::Attack()
{
	const float angleStep{ 2 * (float)M_PI / m_BulletCount };
	float angle{ rand() % 314 / 100.f };

	for (int i{}; i < m_BulletCount; i++)
	{
		Vector2f dir{ cos(angle),sin(angle) };
		angle += angleStep;
		g_BulletManager->ShootBullet(Point2f(m_Position.x+79,m_Position.y+40), dir, 1, true);
		g_SoundManager->PlaySound("FieryShoot.wav");
	}

	m_Attacked = true;
}