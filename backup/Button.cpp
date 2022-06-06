#include "pch.h"
#include "Button.h"
#include "Texture.h"
#include "utils.h"
extern InputS Input;
extern Point2f g_WindowSize;

Button::Button(const std::string& nameSprite, const Point2f& pos) :
	m_Sprite{ new Texture("resources/" + nameSprite) }, m_Position{pos}
{}

Button::~Button()
{
	delete m_Sprite;
}

bool Button::IsHovering() const
{
	return Input.mousePos.x > m_Position.x - m_Sprite->GetWidth()/2 * m_NormalZoom
		&& Input.mousePos.x < m_Position.x - m_Sprite->GetWidth() / 2 * m_NormalZoom + m_Sprite->GetWidth() * m_NormalZoom
		&& g_WindowSize.y - Input.mousePos.y > m_Position.y - m_Sprite->GetHeight() / 2 * m_NormalZoom
		&& g_WindowSize.y - Input.mousePos.y < m_Position.y - m_Sprite->GetHeight() / 2 * m_NormalZoom + m_Sprite->GetHeight() * m_NormalZoom;
}

void Button::Draw() const
{
	glPushMatrix();
	{
		glTranslatef(m_Position.x, m_Position.y, 0);
		glScalef(m_Zoom, m_Zoom,1);
		glTranslatef(-m_Sprite->GetWidth()/2 , -m_Sprite->GetHeight()/2,0);
		m_Sprite->Draw();
	}
	glPopMatrix();
}

void Button::Update()
{
	if (IsHovering())
	{
		m_Zoom = m_NormalZoom * 0.85f;
	}
	else
		m_Zoom = m_NormalZoom;
}

bool Button::IsClicked() const
{
	return IsHovering() && Input.ClickUp == ClickState::MB1;
}