#pragma once

class Texture;
class Button
{
public:
	Button(const std::string& nameSprite, const Point2f& pos);
	~Button();
	bool IsHovering() const;
	bool IsClicked() const;
	void Draw() const;
	void Update();
private:
	float m_NormalZoom{ 5.f };
	float m_Zoom{ m_NormalZoom };
	Point2f m_Position;
	Texture* m_Sprite;
};

