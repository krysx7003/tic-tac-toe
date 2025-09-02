#pragma once

#include "gui_item.h"
#include "text_field.h"

#include <glm/ext/vector_float4.hpp>
#include <memory>
#include <string>
#include <vector>

class Menu : public Gui_Item {

	Text_Field Name;
	bool is_named = false;
	std::vector<std::unique_ptr<Gui_Item>> Items;
	float start_items_y;

  public:
	enum class Position {
		LEFT,
		CENTER,
		RIGHT,

	};

	Menu(int width, int height, int start_pos_x, int start_pos_y, std::string name = "")
		: Gui_Item(width, height, start_pos_x, start_pos_y) {
		if (!name.empty()) {
			int text_start_pos_y = start_pos_y + height - 60;

			Name = Text_Field(width, 60, start_pos_x, text_start_pos_y, name);
			Name.SetBgColor(0.0f, 0.0f, 1.0f);
			is_named = true;
		}
	}
	Menu() {};

	void Draw() override;
	void AddItem(Gui_Item::Type type, float width, float height,
				 const Menu::Position position = Menu::Position::CENTER);
	float GetItemHeight();
	float GetXPad(Menu::Position position, float width);
	float GetYPad(float height);
};
