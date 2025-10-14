#include "gui/widgets/column.h"

Gui_Item *Column::add(Gui_Item::Type type, int width_px, int height_px, std::string name,
					  bool updateVertices, int id) {
	int child_x, child_y;
	glm::ivec2 padding = ChildPadding(width_px, height_px);

	child_x = this->start_pos_x + padding[0];
	child_y = this->start_pos_y + padding[1];

	std::unique_ptr<Gui_Item> new_item =
		createItem(type, width_px, height_px, name, child_x, child_y);

	Gui_Item *ptr = new_item.get();
	if (id == -1) {
		if (updateVertices) {
			UpdateItems(height_px + new_item->GetPadding(), child_y);
		}
		Items.push_back(std::move(new_item));
	} else {
		Items.insert(Items.begin() + id, std::move(new_item));
		UpdateItems(0, child_y);
	}

	return ptr;
}

glm::ivec2 Column::ChildPadding(int width_px, int height_px) {
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

	if (total_y > this->height) {
		printf("WARN::MENU: Gui_Items do not fit container\n");
		return glm::ivec2(0, 0);
	}

	int padding_y = (this->height - total_y) / 2;

	return glm::ivec2(padding_x, padding_y);
}

void Column::UpdateItems(int child_height, int child_pos_y) {
	int total_height = child_height;
	int size = this->Items.size();

	for (int i = size; i > 0; i--) {
		Items[i - 1]->SetStartY(child_pos_y + total_height);
		total_height += Items[i - 1]->GetHeight();
	}
}

void Column::UpdateItems() {
	glm::ivec2 padding = ChildPadding(0, 0);
	UpdateItems(0, this->start_pos_y + padding[1]);
}
