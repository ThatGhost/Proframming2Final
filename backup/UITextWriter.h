#pragma once
#include <vector>

class Texture;


class UITextWriter
{
public:
	UITextWriter();
	~UITextWriter();
	UITextWriter(const UITextWriter& other) = delete;
	UITextWriter& operator=(const UITextWriter& other) = delete;
	UITextWriter(UITextWriter&& other) = delete;
	UITextWriter& operator=(UITextWriter&& other) = delete;

	void DrawAllText();
	void QueText(const std::string& text, const Rectf& Dimensions);
private:
	struct TextData {
		const std::string text;
		const Rectf Dimensions;
	};

	Texture* m_Font{nullptr};
	std::vector<TextData> m_Que{};
	const int m_TextSize{ 20 }; //32

	void DrawUIText(const TextData* data);

};

