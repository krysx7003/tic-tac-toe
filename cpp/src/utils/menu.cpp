#include "menu.h"
#include "button.h"
#include "text_field.h"
#include <cstdio>

void Menu::Draw() {
	this->drawSquare();

	if (is_named) {
		Name.Draw();
	}

	for (auto &item : Items) {
		item->Draw();
	}
}

glm::ivec2 Menu::colChildPadding(int width_px, int height_px) {
	if (width_px > this->width) {
		printf("WARN::MENU: Gui_Items do not fit container\n");

		return glm::ivec2(0, 0);
	}

	int padding_x = (this->width - width_px) / 2.0f;

	int total_y = 0;
	for (auto &item : Items) {
		total_y += item->GetHeight() + this->item_padding_y;
	}
	total_y += height_px;

	if (total_y > this->height - Name.GetHeight()) {
		printf("WARN::MENU: Gui_Items do not fit container\n");
		return glm::ivec2(0, 0);
	}

	int padding_y = (this->height - Name.GetHeight() - total_y) / 2;

	return glm::ivec2(padding_x, padding_y);
}

void Menu::colUpdateItems(int child_height, int child_pos_y) {
	int total_height = child_height + this->item_padding_y;
	int size = this->Items.size();

	for (int i = size; i > 0; i--) {
		Items[i - 1]->SetStartY(child_pos_y + total_height);
		total_height += Items[i - 1]->GetHeight() + this->item_padding_y;
	}
}

void Menu::AddItem(Gui_Item::Type type, int width_px, int height_px, std::string name,
				   bool updateVertices) {
	std::unique_ptr<Gui_Item> new_item;
	int child_x, child_y;
	if (this->layout == Layout::COL) {
		glm::ivec2 padding = colChildPadding(width_px, height_px);

		child_x = this->start_pos_x + padding[0];
		child_y = this->start_pos_y + padding[1];
	} else if (this->layout == Layout::ROW) {
		int total_x = 0;
		for (auto &item : Items) {
			total_x += item->GetWidth() + this->item_padding_x;
		}

		child_x = this->start_pos_x + total_x;
		child_y = this->start_pos_y;
	}

	if (type == Gui_Item::Type::BUTTON) {
		new_item = std::make_unique<Button>(width_px, height_px, child_x, child_y, name);
		new_item->SetBgColor(1.0f, 0.0f, 0.0f);

	} else if (type == Gui_Item::Type::TEXT_FIELD) {
		new_item = std::make_unique<Text_Field>(width_px, height_px, child_x, child_y, name);
		new_item->SetBgColor(0.0f, 0.0f, 1.0f);

	} else {
		printf("ERROR::MENU: Gui_Item is not yet implemented\n");
		exit(-1);
	}

	if (this->layout == Layout::COL && updateVertices) {
		colUpdateItems(height_px, child_y);
	}

	Items.push_back(std::move(new_item));
}

void Menu::SetLayout(Layout new_layout) { this->layout = new_layout; }
