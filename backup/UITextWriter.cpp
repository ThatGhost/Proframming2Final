#include "pch.h"
#include "UITextWriter.h"
#include "Texture.h"

UITextWriter::UITextWriter():
	m_Font{ new Texture("Resources/Font.png") }
{}

UITextWriter::~UITextWriter()
{
	delete m_Font;
}

void UITextWriter::DrawUIText(const TextData* data)
{
	for (int i = 0; i < (int)data->text.size(); i++)
	{
		int letter{ data->text[i] - ' '};
		int x{ letter % 15 };
		int y{letter / 15 + 1};

		m_Font->Draw(Rectf(data->Dimensions.left + i*m_TextSize * data->Dimensions.width, data->Dimensions.bottom,
			m_TextSize * data->Dimensions.width, m_TextSize * data->Dimensions.height),
			Rectf((float)x * m_TextSize, (float)y * m_TextSize, (float)m_TextSize, (float)m_TextSize));
	}
}

void UITextWriter::DrawAllText()
{
	for (TextData e : m_Que)
	{
		DrawUIText(&e);
	}
	m_Que.clear();
}

void UITextWriter::QueText(const std::string& text, const Rectf& Dimensions)
{
	m_Que.push_back(TextData{ text,Dimensions });
}