#pragma once
#include "gui_item.h"
#include "text_renderer.h"
#include <string>

class Text_Field : public Gui_Item {

	TextRenderer *text;
	std::string Text;
	int window_width;
	int window_height;
	int text_offset_y = 24;
	float scale = 1.0f;

  public:
	Text_Field(int width, int height, int start_pos_x, int start_pos_y, std::string text_str)
		: Gui_Item(width, height, start_pos_x, start_pos_y) {
		Text = text_str;

		text = TextRenderer::GetInstance();
	}
	Text_Field() {};

	void Draw() override;
	void SetText(std::string new_text);
	void SetScale(float scale);
};
