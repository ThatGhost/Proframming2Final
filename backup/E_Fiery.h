#pragma once
#include "Enemy.h"
class E_Fiery :
    public Enemy
{
public:
    virtual Rectf GetCollider() const override
        {return Rectf(m_Position.x + 13, m_Position.y, 32, 46);}
    E_Fiery();
protected:
    virtual void Attack(const float& elapsedSec) final;
};

