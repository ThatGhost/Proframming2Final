#pragma once
class Texture;

class Tile
{
public:
	Tile(Texture* t, const Point2f& Pos, const Point2i dest);

	void Draw() const;

private:
	const int m_SizeTile{32};

	Texture* m_Sprite;
	Point2f m_Pos;
	Point2i m_SrcPos;
};

