#include "gui/widgets/row.h"

Gui_Item *Row::add(Gui_Item::Type type, int width_px, int height_px, std::string name,
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

glm::ivec2 Row::ChildPadding(int width_px, int height_px) {
	printf("WARN::ROW: Not yet implemented");
	return glm::ivec2(0, 0);
}

void Row::UpdateItems(int child_height, int child_pos_x) {
	printf("WARN::ROW: Not yet implemented");
}

void Row::UpdateItems() {
	glm::ivec2 padding = ChildPadding(0, 0);
	UpdateItems(0, this->start_pos_y + padding[1]);
}
