#pragma once
#include <vector>
#include "Object.h"
#include "Gun.h"
struct Image;

class Player : public GameObject
{
public:
	Player(const Point2f& pos, Window* Window);
	~Player();

	virtual void Draw() const override;
	virtual void Update(const float& elapsedSec) override;
	virtual void Damage(const int& damage) override;
	virtual Rectf GetCollider() const override;
	virtual void Die() override;

	//Getters
	Point2f GetPosition() const;
	RoomHandler* GetRoom() const { return m_MyRoom; }
	int GetMaxHp() const { return m_MaxHp; }
	Gun* GetGun() const { return m_Guns[m_CurrentGun]; }
	int GetAmountOfGuns() const { return m_Guns.size(); }

	//Setters
	bool AddGun(const std::string& gunName);
	void AddHealth() { m_HP++; };

private:

	enum class ActionState {
		moving = 1,
		idle = 2,
		dead = 3
	};

	//general
	const int m_MaxHp{ 6 };
	const float m_NormalSpeed{ 170 }; //170
	int m_CurrentGun{};
	float m_Speed{ m_NormalSpeed };
	bool m_RunningRight{};
	bool m_Alive{ true };
	std::vector<Gun*> m_Guns{};
	Window* m_Window;
	ActionState m_ActionState{ ActionState::idle };

	//invinc
	bool m_Damaged{};
	float m_InvincTimer{0};
	const float m_InvincTime{1.f};

	//animation
	const float m_FramesPerSec{4};
	float m_AnimTimer{};
	int m_Frame{};
	const int m_NrOfFrames{4};
	const int m_NrOfRows{3};

	//dash
	float m_DashTimer{ 0 };
	const float m_DashTime{ 0.5f };
	const float m_DashSpeed{ 220.f }; //220

	//death
	float m_DeathCirlceSize{};
	const float m_EndTime{ 5.f };
	float m_EndTimer{ m_EndTime };

	//functions
	float GetGunAngle() const;
	float GetGunAngleRad() const;
};


