#pragma once
#include <vector>
class Bullet;
class Texture;
struct Vector2f;

class BulletManager
{
public:
	BulletManager();

	~BulletManager();
	BulletManager(const BulletManager& other) = delete;
	BulletManager& operator=(const BulletManager& other) = delete;
	BulletManager(BulletManager&& other) = delete;
	BulletManager& operator=(BulletManager&& other) = delete;

	Bullet* GetBullet();
	void ShootBullet(const Point2f& pos, const Vector2f& dir,const int& att,const bool& isEnemy);
	void DrawBullets() const;
	void UpdateBullets(const float& elapsedSec);
	void Destruct();

private:

	std::vector<Bullet*> m_Bullets;
	Texture* m_BulletTexture;

	void Resize();
};

