#pragma once
#include "Object.h"
#include "Texture.h"

class Gun;
class Chest : public GameObject 
{
public:
	Chest();

	~Chest() { delete m_HeartTexture; }
	Chest(const Chest& other) = delete;
	Chest& operator=(const Chest& other) = delete;
	Chest(Chest&& other) = delete;
	Chest& operator=(Chest&& other) = delete;

	virtual void Interact() override;
	virtual void Draw() const override;
	virtual void Update(const float& elapsedSec) override;
	virtual Rectf GetCollider() const override { return Rectf(m_Position.x, m_Position.y, 32, 32); }
private:
	bool m_Open{ false };
	Texture* m_HeartTexture{ new Texture("resources/Heart.png") };

	float m_PopUpTimer{ 2.f };
};