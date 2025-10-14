#include "menu.h"
#include "button.h"
#include "dropdown.h"
#include "text_field.h"
#include <cstdio>

void Menu::Draw() {
	if (!Visible)
		return;

	updateDrawCopy();

	background.Draw(BgColor);

	for (auto &item : itemsDraw) {
		item->Draw();
	}
	outline.Draw(OutColor, outlines);
}

void Menu::DrawPopups() {
	if (!Visible)
		return;

	for (int i = itemsDraw.size() - 1; i >= 0; i--) {
		if (Items[i]->ItemType == Type::DROPDOWN)
			dynamic_cast<Dropdown *>(Items[i].get())->DrawPopups();
	}
}

void Menu::updateDrawCopy() {
	itemsDraw.clear();
	for (auto &item : Items) {
		itemsDraw.push_back(item.get());
	}
}

std::unique_ptr<Gui_Item> Menu::createItem(Gui_Item::Type type, int width_px, int height_px,
										   std::string name, int child_x, int child_y) {
	std::unique_ptr<Gui_Item> new_item;

	if (type == Gui_Item::Type::BUTTON) {
		new_item = std::make_unique<Button>(window, width_px, height_px, child_x, child_y, name);
		new_item->SetFgColor(this->FgColor);

	} else if (type == Gui_Item::Type::TEXT_FIELD) {
		new_item = std::make_unique<Text_Field>(width_px, height_px, child_x, child_y, name);
		new_item->SetBgColor(this->BgColor);
		new_item->SetFgColor(this->FgColor);

	} else if (type == Gui_Item::Type::DROPDOWN) {
		new_item = std::make_unique<Dropdown>(window, width_px, height_px, child_x, child_y);
	} else {
		printf("ERROR::MENU: Gui_Item is not yet implemented\n");
		exit(-1);
	}

	return new_item;
}

Gui_Item *Menu::AddItem(Gui_Item::Type type, int width_px, int height_px, std::string name,
						bool updateVertices, int id) {
	Gui_Item *new_item;
	new_item = add(type, width_px, height_px, name, updateVertices, id);

	for (size_t i = 0; i < Items.size(); ++i) {
		Items[i]->Menu_id = i;
	}
	new_item->SetType(type);

	return new_item;
}

void Menu::RemoveItem(int id, bool update) {
	Items.erase(Items.begin() + id);

	for (size_t i = 0; i < Items.size(); ++i) {
		Items[i]->Menu_id = i;
	}

	if (update) {
		UpdateItems();
	}
}

void Menu::SetName(std::string name) { this->Name = name; }
