#include "menu.h"
#include "button.h"
#include "text_field.h"

void Menu::Draw() {
	this->drawSquare();

	if (is_named) {
		Name.Draw();
	}

	for (auto &item : Items) {
		item->Draw();
	}
}

float Menu::GetXPad(Menu::Position position, float child_width) {
	if (position == Menu::Position::LEFT) {
		return this->start_pos_x;
	} else if (position == Menu::Position::RIGHT) {
		return this->width - child_width;
	} else if (position == Menu::Position::CENTER) {
		float pad = (this->width - child_width) / 2;
		return this->width + pad;
	} else {
		printf("ERROR::MENU: Position is not yet implemented\n");
		exit(-1);
	}
}
float Menu::GetItemHeight() {
	if (this->is_named) {
		return start_pos_y - this->Name.GetHeight();
	} else {
		return start_pos_y;
	}
}

float Menu::GetYPad(float height) {
	float item_height = this->GetItemHeight();

	float total_height = 0.0f;
	for (auto &item : Items) {
		total_height = item->GetHeight();
	}
	total_height += height;

	if (total_height < item_height) {
		float pad = (item_height - total_height) / 2;
		return item_height - pad;
	} else if (total_height == item_height) {
		return item_height;
	} else {
		printf("ERROR::MENU: Items do not fit inside menu\n");
		exit(-1);
	}
}

void Menu::AddItem(Gui_Item::Type type, float width, float height, const Menu::Position position) {
	std::unique_ptr<Gui_Item> new_item;
	if (width > this->width) {
		printf("ERROR::MENU: Gui_Item out of bounds\n");
		exit(-1);
	}
	if (height > this->height) {
		printf("ERROR::MENU: Gui_Item out of bounds\n");
		exit(-1);
	}

	if (type == Gui_Item::Type::BUTTON) {
		new_item = std::make_unique<Button>(width, height, 0, 0, "Test");

	} else if (type == Gui_Item::Type::TEXT_FIELD) {
		new_item = std::make_unique<Text_Field>(width, height, 0, 0, "Test");

	} else {
		printf("ERROR::MENU: Gui_Item is not yet implemented\n");
		exit(-1);
	}
	Items.push_back(std::move(new_item));
}
