#pragma once
#include "gui_item.h"
#include "text_field.h"
#include <GLFW/glfw3.h>

class Button : public Gui_Item {
	GLFWwindow *window;
	void (*on_clik_func)();
	bool wasMousePressed = false;
	bool isClickHandled = false;

	bool isMouseOn();

  public:
	Text_Field button_text;
	Button(GLFWwindow *window, int width, int height, int start_pos_x, int start_pos_y,
		   std::string text)
		: Gui_Item(width, height, start_pos_x, start_pos_y), window(window),
		  button_text(width, height, start_pos_x, start_pos_y, text) {

		this->item_text = text;
		this->SetOutline(glm::bvec4(true));
		this->SetBgColor("#1e1f22");
		button_text.SetBgColor(this->BgColor);
		button_text.SetAlignmentHor(AlignmentHor::CENTER);
	}

	void Draw() override;
	void On_Click();

	void SetStartY(int start_y) override;
	void SetOnClick(void (*func)());
};
