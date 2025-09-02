#pragma once
#include "gui_item.h"
#include <string>

class Text_Field : public Gui_Item {
	std::string Text;

  public:
	Text_Field(float width, float height, float start_pos_x, float start_pos_y, std::string text)
		: Gui_Item(width, height, start_pos_x, start_pos_y) {
		// Implement text stuff
	}
	Text_Field() {};

	void Draw() override;
	void SetText(std::string new_text);
	void SetBackground(glm::vec3 color);
};
