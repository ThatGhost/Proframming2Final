#pragma once
struct Vector2f;

class BasicBehaviour
{
public:
	virtual ~BasicBehaviour() {}

	Point2f ScreenToWorldPoint(const Point2f& p) const;
	Vector2f GetNormDirectionBetweenPoints(const Point2f& p1, const Point2f& p2) const;

	float GetDistBetweenPoints(const Point2f& p1, const Point2f& p2) const;
	float GetAngleBetweenPoints(const Point2f& p1, const Point2f& p2) const;
	int sgn(float val);
protected:
	const double RadToDegree{180.0/M_PI};
	const double DegreeToRad{M_PI/180.0};
};

