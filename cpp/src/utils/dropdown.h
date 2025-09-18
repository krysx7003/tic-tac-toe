#pragma once

#include "button.h"
#include "gui_item.h"
#include "rect.h"

#include <GLFW/glfw3.h>
#include <cstdio>
#include <vector>

class Dropdown : public Gui_Item {
  protected:
	GLFWwindow *window;
	std::vector<Button> items;
	Button currItem;

	Rect dropdown;
	Line dropdown_outline;
	bool dropdown_visible;

	void updatePos() override;
	void setCurrent(int id);
	void createBox(int width, int height, int start_pos_x, int start_pos_y);
	void toggleDropdown();

  public:
	Dropdown(GLFWwindow *window, int width, int height, int start_pos_x, int start_pos_y)
		: Gui_Item(width, height, start_pos_x, start_pos_y), window(window) {

		this->SetOutColor("#FFFFFF");

		currItem = Button(window, width, height, start_pos_x, start_pos_y, "");
		currItem.SetOnClick([this]() { this->toggleDropdown(); });
		currItem.SetOutline(glm::vec4(true));
	}
	Dropdown() {};

	void Draw() override;
	void DrawPopups();

	void AddItems(std::vector<std::string> names);

	void SetDropdown(bool visible);
	std::string GetSelected();
};
