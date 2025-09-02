#pragma once
#include "gui_item.h"
#include "text_field.h"

class Button : public Gui_Item {
	Text_Field button_text;

  public:
	Button(float width, float height, float start_pos_x, float start_pos_y, std::string text)
		: Gui_Item(width, height, start_pos_x, start_pos_y),
		  button_text(width, height, start_pos_x, start_pos_y, text) {}

	void Draw() override;
	void On_Click();
};
