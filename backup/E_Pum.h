#pragma once
#include "Enemy.h"
class E_Pum :
    public Enemy
{
public:
    virtual Rectf GetCollider() const override
        {return Rectf(m_Position.x + 5, m_Position.y + 3, 20, 26);}
    E_Pum();
};

