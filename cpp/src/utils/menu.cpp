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
		total_y += item->GetHeight();
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
	int total_height = child_height;
	int size = this->Items.size();

	for (int i = size; i > 0; i--) {
		Items[i - 1]->SetStartY(child_pos_y + total_height);
		total_height += Items[i - 1]->GetHeight();
	}
}

Gui_Item *Menu::addToColl(Gui_Item::Type type, int width_px, int height_px, std::string name,
						  bool updateVertices, int id) {
	int child_x, child_y;
	glm::ivec2 padding = colChildPadding(width_px, height_px);

	child_x = this->start_pos_x + padding[0];
	child_y = this->start_pos_y + padding[1];

	std::unique_ptr<Gui_Item> new_item =
		createItem(type, width_px, height_px, name, child_x, child_y);

	Gui_Item *ptr = new_item.get();
	if (id == -1) {
		if (updateVertices) {
			colUpdateItems(height_px + new_item->GetPadding(), child_y);
		}

		Items.push_back(std::move(new_item));
	} else {
		Items.insert(Items.begin() + id, std::move(new_item));
		colUpdateItems(0, child_y);
	}

	return ptr;
}

Gui_Item *Menu::addToRow(Gui_Item::Type type, int width_px, int height_px, std::string name,
						 bool updateVertices, int id) {
	int child_x, child_y;

	int total_x = 0;
	for (auto &item : Items) {
		total_x += item->GetWidth() + this->item_padding_x;
	}

	child_x = this->start_pos_x + total_x;
	child_y = this->start_pos_y;

	std::unique_ptr<Gui_Item> new_item =
		createItem(type, width_px, height_px, name, child_x, child_y);

	Gui_Item *ptr = new_item.get();
	if (id == -1) {
		Items.push_back(std::move(new_item));
	} else {
		printf("ERROR::MENU: Adding items at id is not yet impelmented\n");
		exit(-1);
	}
	return ptr;
}

std::unique_ptr<Gui_Item> Menu::createItem(Gui_Item::Type type, int width_px, int height_px,
										   std::string name, int child_x, int child_y) {
	std::unique_ptr<Gui_Item> new_item;

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
	return new_item;
}

Gui_Item *Menu::AddItem(Gui_Item::Type type, int width_px, int height_px, std::string name,
						bool updateVertices, int id) {
	Gui_Item *new_item;
	if (this->layout == Layout::COL) {
		new_item = addToColl(type, width_px, height_px, name, updateVertices, id);

	} else if (this->layout == Layout::ROW) {
		new_item = addToRow(type, width_px, height_px, name, updateVertices, id);
	}

	if (id == -1) {
		new_item->Menu_id = Items.size() - 1;

	} else {
		new_item->Menu_id = id;
	}

	return new_item;
}

void Menu::RemoveItem(int id) { Items.erase(Items.begin() + id); }

void Menu::SetLayout(Layout new_layout) { this->layout = new_layout; }
