#pragma once
#include "Object.h"

class Gun : public GameObject
{
public:
	Gun(const std::string& gunName);

	virtual ~Gun() override;
	Gun(const Gun& other) = delete;
	Gun& operator=(const Gun& other) = delete;
	Gun(Gun&& other) = delete;
	Gun& operator=(Gun&& other) = delete;

	void Draw(const float& angle, const Point2f& pos);
	void Shoot(const Vector2f& dir);
	void Reload();
	virtual void Update(const float& elapsedSec) override;
	int GetAmmoInClip() const { return m_AmmoInClip; }
	int GetMaxAmmo() const { return m_MaxAmmo; }
	int GetBullets() const { return m_Bullets; }
	int GetMaxBullets() const { return m_Maxbullets; }
	Texture* GetIcon() const { return m_Sprite; }
	std::string GetGunName() const { return m_GunName; }
private:
	int m_MaxAmmo{};
	std::string m_GunName{};
	float m_ShootSpeed{};
	float m_ShootTimer{};
	int m_AmmoInClip{};
	int m_Bullets{};
	int m_Maxbullets{};
	int m_Att{};
};

