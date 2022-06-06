#pragma once
#include "Object.h"
#include "Texture.h"

enum class BossState {
    Idle = 1,
    Attacking = 2,
    Death = 3
};

class Boss :
    public GameObject
{
public:
    Boss();
    virtual ~Boss() override;

    virtual void Draw() const override;
    virtual void Update(const float& elapsedSec) override;
    virtual Rectf GetCollider() const override { return Rectf(m_Position.x + 30,m_Position.y + 14,m_FrameSize - 40,m_FrameSize-25); };

private:
    //general
    BossState m_State{BossState::Idle};
    int m_ActualMaxHealth{INT16_MAX};
    bool m_Attacked{ false };
    bool m_Won{ false };
    int m_MaxHealth{ 150 };
    float m_ElipseSize{ 2 };

    //Animations
    const float m_AnimationSpeed{0.2f};
    float m_AnimTimer{};
    int m_Frame{};
    const float m_FrameSize{ 100 };
    const int m_AttackFrameAmount{ 12 };
    const int m_DeathFrameAmount{ 18 };
    const int m_IdleFrameAmount{ 4 };
    
    //textures
    Texture* m_Attack{new Texture("resources/objects/boss/attacking.png")};
    Texture* m_Death{new Texture("resources/objects/boss/Death.png")};
    Texture* m_Idle{new Texture("resources/objects/boss/idle.png")};

    //aI
    const float m_AttackTime{1.6f};
    float m_AttackTimer{ m_AttackTime };
    int m_BulletCount{15};

    //returning to MainMenu
    float m_EndTimer{ 5.f };

    //functions
    void Attack();
};

