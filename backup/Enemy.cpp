#include "pch.h"
#include "Enemy.h"
#include <cmath>
#include "Texture.h"
#include "Image.h"
#include "Player.h"
#include "GameVariables.h"
#include "utils.h"

using namespace utils;

Enemy::Enemy(const std::string& Sname, int HP, float movementspeed, float shootSpeed) :
	GameObject(Sname, Type::Enemy),
	m_Speed{movementspeed},
	m_ShootSpeed{shootSpeed}
{
	m_HP = HP;
}

void Enemy::Draw() const {
	if (m_MyRoom != g_Player->GetRoom()) return;
	Point2f sizeOfFrame{ m_Sprite->GetWidth() / (m_NrOfFrames * 2) ,m_Sprite->GetHeight() };

	int xOffset{};
	if (m_State == State::closing || m_State == State::running) 
	{
		xOffset = 4 * (int)sizeOfFrame.x;
	}

	Rectf src{(float)xOffset + m_Frame * sizeOfFrame.x, sizeOfFrame.y, sizeOfFrame.x, sizeOfFrame.y};
	glPushMatrix();
	{
		glTranslatef(m_Position.x, m_Position.y, 0);
		//look to left when going left
		if (m_RunningRight)
		{
			glScalef(1.5f, 1.5f, 0);
			glTranslatef(0, 0, 0);
		}
		else
		{
			glScalef(-1.5f, 1.5f, 0);
			glTranslatef(-sizeOfFrame.x, 0, 0);
		}
		m_Sprite->Draw(Point2f(0, 0), src);
	}
	glPopMatrix();

	//if (m_Path.size() > 1)
	//{
	//	std::list<Node> copyList{ m_Path };
	//	Point2f prev{ m_Position };
	//	while (copyList.size() > 0)
	//	{
	//		Point2f next{ (copyList.front().pos.x + m_MyRoom->GetPosition().x) * (float)g_SizeTiles, (copyList.front().pos.y + m_MyRoom->GetPosition().y) * (float)g_SizeTiles };
	//		utils::DrawLine(prev, next);
	//		prev = next;
	//		copyList.pop_front();
	//	}
	//}
}

void Enemy::Update(const float& elapsedSec) 
{
	if (g_Player->GetRoom() != m_MyRoom) return;

	if (m_FoundPlayer)
	{
		//pathfinding a*
		if (m_PathTimer <= 0) {
			RunAStar();
			m_PathTimer = m_PathTime;
		}
		else m_PathTimer -= elapsedSec;

		//follow A*
		if (m_Path.size() > 1)
		{
			Point2f frontPath{ (float)(m_MyRoom->GetPosition().x + m_Path.front().pos.x) * g_SizeTiles ,
							   (float)(m_MyRoom->GetPosition().y + m_Path.front().pos.y) * g_SizeTiles };

			if (abs(m_Position.x - frontPath.x) < 1 &&
				abs(m_Position.y - frontPath.y) < 1)
			{
				m_Path.pop_front();
			}

			float signx{ frontPath.x - m_Position.x };
			if (signx < -1)m_RunningRight = false;
			else if (signx > 1) m_RunningRight = true;

			m_Position.x = m_Position.x + (sgn(frontPath.x - m_Position.x) * m_Speed * elapsedSec);
			m_Position.y = m_Position.y + (sgn(frontPath.y - m_Position.y) * m_Speed * elapsedSec);

			//attacking behaviour
			if (m_AttackTimer <= 0)
			{
				Attack(elapsedSec);
				m_AttackTimer = m_ShootSpeed;
			}
			else
				m_AttackTimer -= elapsedSec;
		}
	}
	else
	{
		RayCastFindPlayer();
		//wander
	}

	//Animations timer
	if (m_AnimationTimer > 0) m_AnimationTimer -= elapsedSec;
	else
	{
		m_Frame++;
		if (m_Frame == m_NrOfFrames)m_Frame = 0;
		m_AnimationTimer = 1.0f / m_FramesPerSec;
	}

	Vector2f dirToPlayer{ GetNormDirectionBetweenPoints(m_Position,g_Player->GetPosition()) };
	float distToPlayer{ GetDistBetweenPoints(m_Position , g_Player->GetPosition()) };
	
	if (dirToPlayer.x < 0) m_RunningRight = false;
	else if (dirToPlayer.x > 0) m_RunningRight = true;
}

void Enemy::Attack(const float& elapsedSec) 
{
	Vector2f dir{ GetNormDirectionBetweenPoints(m_Position, g_Player->GetPosition()) };
	g_BulletManager->ShootBullet(m_Position, dir, 1, true);
	g_SoundManager->PlaySound("GrumShoot.wav");
}

void Enemy::RayCastFindPlayer()
{
	Rectf playerCollider{g_Player->GetCollider()};
	std::vector<Point2f> vert{};
	vert.push_back(Point2f(playerCollider.left, playerCollider.bottom));
	vert.push_back(Point2f(playerCollider.left + playerCollider.width, playerCollider.bottom));
	vert.push_back(Point2f(playerCollider.left + playerCollider.width, playerCollider.bottom + playerCollider.height));
	vert.push_back(Point2f(playerCollider.left, playerCollider.bottom + playerCollider.height));

	const float stepAmount{ 100 };
	const float stepSize{360.f / stepAmount };
	const float rayLenght{200.f};
	for (size_t i = 0; i < stepAmount; i++)
	{
		HitInfo hit{};
		Point2f Dir{cosf(stepSize * i) * rayLenght, sinf(stepSize * i) * rayLenght };
		if (Raycast(vert, m_Position, m_Position + Dir, hit)) {
			m_FoundPlayer = true;
			m_State = State::running;
		}
	}
}

void Enemy::RunAStar()
{
	//const float MaxDistance{300.f};
	if (g_Player->GetRoom() == m_MyRoom)
	{
		Node start{};
		start.pos.x = int(m_Position.x / g_SizeTiles - m_MyRoom->GetPosition().x);
		start.pos.y = int(m_Position.y / g_SizeTiles - m_MyRoom->GetPosition().y);

		Node destination{};
		destination.pos.x = int(g_Player->GetPosition().x / g_SizeTiles - m_MyRoom->GetPosition().x);
		destination.pos.y = int(g_Player->GetPosition().y / g_SizeTiles - m_MyRoom->GetPosition().y);

		aStar(&start, &destination);
	}
}

void Enemy::aStar(Node* start, Node* dest) 
{
	std::vector<Node> map{};
	Image* roomImage{ m_MyRoom->GetImage() };
	for (int y = 0; y < roomImage->h + 2; y++)
	{
		for (int x = 0; x < roomImage->w; x++)
		{
			map.push_back(Node(Point2i(x, y), roomImage->data[((roomImage->h - y) * roomImage->w + x) * 3] == 33));
		}
	}

	for (int y = 0; y < roomImage->h + 2; y++)
	{
		for (int x = 0; x < roomImage->w; x++)
		{
			int index{ y * roomImage->w + x};
			if (y > 0)
				map[y * roomImage->w + x].neighbours.push_back(&map[(y - 1) * roomImage->w + (x + 0)]);
			if (y < roomImage->h - 1)
				map[y * roomImage->w + x].neighbours.push_back(&map[(y + 1) * roomImage->w + (x + 0)]);
			if (x > 0)
				map[y * roomImage->w + x].neighbours.push_back(&map[(y + 0) * roomImage->w + (x - 1)]);
			if (x < roomImage->w - 1)
				map[y * roomImage->w + x].neighbours.push_back(&map[(y + 0) * roomImage->w + (x + 1)]);
		}
	}
	std::list<Node*> listNotTested{};

	start = &map[start->pos.y * roomImage->w + start->pos.x];
	dest = &map[dest->pos.y * roomImage->w + dest->pos.x];

	start->localCost = 0;
	start->globalCost = abs(distance(start, dest));

	listNotTested.push_back(start);
	Node* nodeCurrent{start};

	while (!listNotTested.empty() && !(nodeCurrent->pos == dest->pos))
	{
		listNotTested.sort([](const Node* lhs, const Node* rhs) { return lhs->globalCost < rhs->globalCost; });

		while (!listNotTested.empty() && listNotTested.front()->isVisisted)
			listNotTested.pop_front();

		if (listNotTested.empty())break;

		nodeCurrent = listNotTested.front();
		nodeCurrent->isVisisted = true;

		for (auto neighbour : nodeCurrent->neighbours)
		{
			if (!neighbour->isVisisted && !neighbour->isObstacle)
				listNotTested.push_back(neighbour);
			float newlocalCost{ nodeCurrent->localCost + 1};

			if (newlocalCost < neighbour->localCost)
			{
				neighbour->parent = nodeCurrent;
				neighbour->localCost = newlocalCost;

				neighbour->globalCost = neighbour->localCost + abs(distance(neighbour, dest));
			}
		}
	}

	if (dest->parent != nullptr)
	{
		m_Path.clear();
		nodeCurrent = dest;
		while (nodeCurrent->parent != nullptr)
		{
			m_Path.push_front(Node(nodeCurrent->pos));
			nodeCurrent = nodeCurrent->parent;
		}
	}
}

float Enemy::distance(const Node& a,const Node& b)
{
	return float(sqrtf((a.pos.x - b.pos.x) * (a.pos.x - (float)b.pos.x) + (a.pos.y - b.pos.y) * (a.pos.y - b.pos.y)));
};