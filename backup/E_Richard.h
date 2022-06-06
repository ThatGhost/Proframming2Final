#pragma once
#include "Enemy.h"
class E_Richard :
    public Enemy
{
public:
    virtual Rectf GetCollider() const override 
        { return Rectf(m_Position.x + 7, m_Position.y, 32, 42); }
    E_Richard();
protected:
    virtual void Attack(const float& elapsedSec) final;
};

