#pragma once
#include "Object.h"

class Table : public GameObject
{
public:
	Table();
	virtual void Interact() override;
	virtual void Draw() const override;
	virtual Rectf GetCollider() const override { return Rectf(m_Position.x, m_Position.y, 64, 32); }
private:
	bool m_Trown{false};
};