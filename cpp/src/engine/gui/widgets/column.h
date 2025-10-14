#pragma once

#include "gui/widgets/menu.h"

class Column : public Menu {

  public:
	Column(GLFWwindow *window, int width, int height, int start_pos_x, int start_pos_y)
		: Menu(window, width, height, start_pos_x, start_pos_y) {}

	Column() {};

	Gui_Item *add(Gui_Item::Type type, int width_px, int height_px, std::string name,
				  bool updateVertices, int id) override;

	glm::ivec2 ChildPadding(int width_px, int height_px) override;
	void UpdateItems(int child_height, int child_pos_x) override;
	void UpdateItems() override;
};
