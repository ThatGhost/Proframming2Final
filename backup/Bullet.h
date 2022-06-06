#pragma once
#include "Object.h"
#include "Vector2f.h"

class Bullet :
    public GameObject
{
public:
    Bullet(float damage, Texture* t);

    virtual void Draw() const override;
    virtual void Update(const float& elapsedSec) override;
    void SetTrajectory(const Point2f& pos,const Vector2f& dir, const float& speed, const float& destoryTime = 5.f);
    void SetFromEnemy(bool fromEnemy) { m_FromEnemy = fromEnemy; }
    bool IsFromEnemy() const { return m_FromEnemy; }
    void SetDamage(const int& damage) { m_Damage = damage; }
    virtual Rectf GetCollider() const override { return Rectf(m_Position.x, m_Position.y, m_Sprite->GetWidth(), m_Sprite->GetHeight()); }
private:
    int m_Damage;
    bool m_Active{ false };
    bool m_FromEnemy{ true };
    float m_Timer{0.f};

    float m_Speed;
    Vector2f m_Direction;
};

