#pragma once
#include "Object.h"

class DungeonDoor :
    public GameObject
{
public:
    DungeonDoor(std::string name = "Door.png");
    void UnlockDoor(const bool& toggle);
    bool IsUnlocked() const { return m_Unlocked; }
    virtual void Draw() const override;
    virtual Rectf GetCollider() const override { return Rectf(m_Position.x, m_Position.y, 64, 32); }
private:
    bool m_Unlocked;
    //TODO - collisions
};

