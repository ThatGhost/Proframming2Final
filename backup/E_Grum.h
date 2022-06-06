#pragma once
#include "Enemy.h"
class E_Grum :
    public Enemy
{
public:
    virtual Rectf GetCollider() const override
        {return Rectf(m_Position.x + 5, m_Position.y + 3, 20, 15);}
    E_Grum();
protected:
    virtual void Attack(const float& elapsedSec) final;
};

