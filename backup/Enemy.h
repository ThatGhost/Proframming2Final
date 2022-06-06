#pragma once
#include "Object.h"
#include <vector>
#include <list>

struct Node;

class Enemy :
    public GameObject
{
public:
    Enemy(const std::string& Sname, int HP, float movementspeed, float shootSpeed);

    virtual void Draw() const override;
    virtual void Update(const float& elapsedSec) override;

protected:
    float m_Speed;
    float m_ShootSpeed;
    virtual void Attack(const float& elapsedSec);
    void RayCastFindPlayer();
    float m_IdleDist{ 150 }, m_RunningDist{ 75 };

private:
    enum class State {
        idle = 0,
        running = 1,
        closing = 2,
        dodging = 3
    };

    const int m_NrOfFrames{ 4 };

    //Point2f m_SizeOfFrame;

    State m_State{ State::idle };
    bool m_RunningRight{false};
    bool m_FoundPlayer{ false };

    float m_PathTimer{};
    float m_PathTime{1.5f};

    float m_AttackTimer{ 0.f };

    float m_AnimationTimer{ 0.f };
    float m_FramesPerSec{4};
    int m_Frame{ 0 };

    std::list<Node> m_Path;

    void RunAStar();
    void aStar(Node* start, Node* dest);
    float distance(const Node& a, const Node& b);
};

struct Node
{
    Node(const Node* rhs)
    {
        this->pos = rhs->pos;
        this->globalCost = rhs->globalCost;
        this->localCost = rhs->globalCost;
        this->neighbours = rhs->neighbours;
    }
    Node(const Point2i& pos = Point2i(), const bool& isObstacle = false) {
        this->pos = pos;
        this->isObstacle = isObstacle;
        this->globalCost = INT16_MAX ;
        this->localCost = INT16_MAX;
    }
    Point2i pos;
    Node* parent{nullptr};
    bool isVisisted{};
    bool isObstacle;
    std::vector<Node*> neighbours{};
    float globalCost{};
    float localCost{};
};

