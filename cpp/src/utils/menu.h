#pragma once

#include "gui_item.h"
#include "text_field.h"

#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_int2.hpp>
#include <memory>
#include <string>
#include <vector>

class Menu : public Gui_Item {

	Text_Field Name;
	bool is_named = false;
	float start_items_y;
	int item_padding_x = 30;

	Gui_Item *addToColl(Gui_Item::Type type, int width_px, int height_px, std::string name,
						bool updateVertices, int id);
	Gui_Item *addToRow(Gui_Item::Type type, int width_px, int height_px, std::string name,
					   bool updateVertices, int id);
	std::unique_ptr<Gui_Item> createItem(Gui_Item::Type type, int width_px, int height_px,
										 std::string name, int child_x, int child_y);

  public:
	enum class Layout { ROW, COL };
	Menu::Layout layout = Layout::COL;
	std::vector<std::unique_ptr<Gui_Item>> Items;

	Menu(int width, int height, int start_pos_x, int start_pos_y, std::string name = "",
		 Layout layout = Layout::COL)
		: Gui_Item(width, height, start_pos_x, start_pos_y), layout(layout) {
		if (!name.empty()) {
			int text_start_pos_y = start_pos_y + height - 60;

			Name = Text_Field(width, 60, start_pos_x, text_start_pos_y, name);
			Name.SetBgColor(0.0f, 0.0f, 1.0f);
			is_named = true;
		}
	}
	Menu() {};

	void Draw() override;
	Gui_Item *AddItem(Gui_Item::Type type, int width_px, int height_px, std::string name,
					  bool updateVertices = true, int id = -1);
	void RemoveItem(int id);
	void SetLayout(Layout new_layout);
	glm::ivec2 colChildPadding(int width_px, int height_px);
	void colUpdateItems(int child_height, int child_pos_x);
};
