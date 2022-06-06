#pragma once
#include "Enemy.h"

struct Image;

class E_Mini :
    public Enemy
{
public:
    virtual Rectf GetCollider() const override
        {return Rectf(m_Position.x+5, m_Position.y+3, 20, 26);}
    E_Mini();
private:
    virtual void Attack(const float& elapsedSec) final;
};

