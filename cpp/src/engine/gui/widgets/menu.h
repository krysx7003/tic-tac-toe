#pragma once

#include "gui/gui_item.h"

#include <GLFW/glfw3.h>
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_int2.hpp>
#include <memory>
#include <string>
#include <vector>

class Menu : public Gui_Item {

	GLFWwindow *window;
	float start_items_y;
	int item_padding_x = 30;
	std::vector<Gui_Item *> itemsDraw;

	Gui_Item *addToColl(Gui_Item::Type type, int width_px, int height_px, std::string name,
						bool updateVertices, int id);
	Gui_Item *addToRow(Gui_Item::Type type, int width_px, int height_px, std::string name,
					   bool updateVertices, int id);
	std::unique_ptr<Gui_Item> createItem(Gui_Item::Type type, int width_px, int height_px,
										 std::string name, int child_x, int child_y);
	void updateDrawCopy();

  public:
	enum class Layout { ROW, COL };
	Menu::Layout layout = Layout::COL;
	std::string Name;
	std::vector<std::unique_ptr<Gui_Item>> Items;

	Menu(GLFWwindow *window, int width, int height, int start_pos_x, int start_pos_y)
		: Gui_Item(width, height, start_pos_x, start_pos_y), window(window) {
		this->SetOutlineWidth(3.0f);
	}
	Menu() {};

	void Draw() override;
	void DrawPopups();

	Gui_Item *AddItem(Gui_Item::Type type, int width_px, int height_px, std::string name = " ",
					  bool updateVertices = false, int id = -1);
	void RemoveItem(int id, bool update);
	void UpdateItems();

	void SetLayout(Layout new_layout);
	void SetName(std::string name);
	glm::ivec2 colChildPadding(int width_px, int height_px);
	void colUpdateItems(int child_height, int child_pos_x);
};
