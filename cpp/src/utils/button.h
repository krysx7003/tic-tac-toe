#pragma once
#include "gui_item.h"
#include "text_field.h"

class Button : public Gui_Item {

  public:
	Text_Field button_text;
	Button(int width, int height, int start_pos_x, int start_pos_y, std::string text)
		: Gui_Item(width, height, start_pos_x, start_pos_y),
		  button_text(width, height, start_pos_x, start_pos_y, text) {
		this->SetBgColor(1.0f, 0.0f, 0.0f);
		button_text.SetBgColor(1.0f, 0.0f, 0.0f);
		button_text.SetAlignmentHor(AlignmentHor::CENTER);
	}

	void Draw() override;
	void SetStartY(int start_y) override;
	void On_Click();
};
