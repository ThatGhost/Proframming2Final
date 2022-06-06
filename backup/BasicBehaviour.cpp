#include "pch.h"
#include "BasicBehaviour.h"
#include "GameVariables.h"
#include "Matrix2x3.h"
#include "utils.h"

Point2f BasicBehaviour::ScreenToWorldPoint(const Point2f& p) const
{
	Vector2f newP{p.x  / Camera::Get().GetZoom(), p.y / Camera::Get().GetZoom()};
	newP -= Camera::Get().GetPos() / Camera::Get().GetZoom();

	return Point2f(newP.x, newP.y);
}

float BasicBehaviour::GetDistBetweenPoints(const Point2f& p1, const Point2f& p2) const 
{
	return (float)sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) * 1.0);
}

Vector2f BasicBehaviour::GetNormDirectionBetweenPoints(const Point2f& p1, const Point2f& p2) const
{
	Vector2f v{ p2.x - p1.x, p2.y - p1.y };
	return v.Normalized();
}

float BasicBehaviour::GetAngleBetweenPoints(const Point2f& p1, const Point2f& p2) const
{
	float deltaY{ p1.y - p2.y };
	float deltaX{ p1.x - p2.x };

	return atan2f(deltaY, deltaX);
}

int BasicBehaviour::sgn(float val) {
	return (val > 0) ? 1 : ((val < 0) ? -1 : 0);
}