#pragma once
#include "BasicBehaviour.h"
#include "Texture.h"
#include "RoomHandler.h"
#include "Vector2f.h"

struct RayInfo 
{
    Vector2f intersectPoint;
    GameObject** HitObject{nullptr};
};

enum class Type {
    Enemy,
    Door,
    Interactable,
    Player
};

class GameObject :
    public BasicBehaviour
{
public:
    GameObject(const std::string& name = "empty", Type type = Type::Enemy) : GameObject(name, Point2f(0, 0),type) { };
    GameObject(const std::string& spriteName,Point2f p, Type type = Type::Enemy)
        : m_Position{ p }, name{ spriteName }, m_Active{ true }, m_Type{type}
    { Start(); }
    
    //destructors
    virtual ~GameObject() {}
    GameObject(const GameObject& other) = delete;
    GameObject& operator=(const GameObject& other) = delete;
    GameObject(GameObject&& other) = delete;
    GameObject& operator=(GameObject&& other) = delete;

    //functions
    void AddTexture(Texture* sprite) { m_Sprite = sprite; }
    void SetPosition(const Point2f& pos) { m_Position = pos; }
    void SetHp(const int& hp) { m_HP = hp; }
    void SetActive(const bool& active) { m_Active = active; };
    void SetRoom(RoomHandler* room) { m_MyRoom = room; }
    
    //return functions
    RoomHandler* GetRoom() const { return m_MyRoom; }
    int GetHP() const{ return m_HP; }
    bool IsActive() const { return m_Active; };
    virtual Rectf GetCollider() const { return Rectf(); }
    bool IsCollidingWithWall(const Rectf& collider);
    Type GetType() const { return m_Type; }

    //virtual functions
    virtual void Damage(const int& damage);
    virtual void Start() {}
    virtual void Draw() const { m_Sprite->Draw(m_Position); }
    virtual void Update(const float& elapsedSec) {}
    virtual void Interact() {}
    virtual void Die() { SetActive(false); }

    //TODO -- make get and set
    std::string name{ "empty" };
protected:
    Type m_Type;
    RoomHandler* m_MyRoom{nullptr};
    bool m_Active{};
    Point2f m_Position;
    int m_HP;
    Texture* m_Sprite{nullptr};
};

