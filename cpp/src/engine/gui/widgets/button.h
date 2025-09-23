#pragma once
#include "gui/gui_item.h"
#include "text_field.h"
#include <GLFW/glfw3.h>

class Button : public Gui_Item {
	GLFWwindow *window;
	std::function<void()> on_click_func;
	glm::vec4 HiColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.25f);

	bool isMouseOn();

  public:
	Text_Field button_text;
	Button(GLFWwindow *window, int width, int height, int start_pos_x, int start_pos_y,
		   std::string text)
		: Gui_Item(width, height, start_pos_x, start_pos_y), window(window),
		  button_text(width, height, start_pos_x, start_pos_y, text) {

		this->item_text = text;
		SetOutline(glm::bvec4(true));
		SetOutlineWidth(1.0f);
		SetBgColor("#1e1f22");
		SetOutColor("#FFFFFF");

		button_text.SetBgColor(this->BgColor);
		button_text.SetAlignmentHor(AlignmentHor::CENTER);
	}
	Button() {};

	void Draw() override;
	bool Handle();
	void On_Click();

	void SetBgColor(std::string color) override;
	void SetBgColor(glm::vec4 color) override;
	void SetFgColor(std::string color) override;
	void SetFgColor(glm::vec3 color) override;
	void SetHiColor(std::string hex_color);

	void SetText(std::string text);
	void SetStartY(int start_y) override;
	void SetOnClick(std::function<void()> func);

	std::string GetText();
};
