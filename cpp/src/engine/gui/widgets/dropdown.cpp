#include "dropdown.h"

void Dropdown::setCurrent(int id) {
	std::string text = items[id].GetText();
	currItem.SetText(text);
	this->dropdown_visible = false;
}

void Dropdown::toggleDropdown() { this->dropdown_visible = !this->dropdown_visible; }

void Dropdown::createBox(int box_width, int box_height, int box_start_pos_x, int box_start_pos_y) {
	float fWidth = (box_width / (window_width / 2.0f));
	float fHeight = (box_height / (window_height / 2.0f));
	float fStart_pos_x = (box_start_pos_x / (window_width / 2.0f)) - 1;
	float fStart_pos_y = (box_start_pos_y / (window_height / 2.0f)) - 1;

	glm::vec3 bottom_left = {fStart_pos_x, fStart_pos_y, 0.0f};
	glm::vec3 bottom_right = {fStart_pos_x + fWidth, fStart_pos_y, 0.0f};
	glm::vec3 top_left = {fStart_pos_x, fStart_pos_y + fHeight, 0.0f};
	glm::vec3 top_right = {fStart_pos_x + fWidth, fStart_pos_y + fHeight, 0.0f};

	dropdown.Init(bottom_left, bottom_right, top_left, top_right);

	std::vector<glm::vec3> outline_vert;
	outline_vert.push_back(top_left);
	outline_vert.push_back(top_right);
	outline_vert.push_back(bottom_right);
	outline_vert.push_back(bottom_left);
	outline_vert.push_back(top_left);

	dropdown_outline.Init(outline_vert);
	dropdown_outline.SetWidth(1.0f);
}

void Dropdown::Draw() {
	if (!Visible)
		return;

	currItem.Draw();
}

void Dropdown::DrawPopups() {
	if (!Visible)
		return;

	if (dropdown_visible) {
		dropdown.Draw(BgColor);

		for (Button &item : items) {
			item.Draw();
		}

		dropdown_outline.Draw(OutColor, glm::vec4(true));
	}
}

void Dropdown::updatePos() {
	currItem.SetStartY(this->start_pos_y);

	int child_start_y = this->start_pos_y;
	int total_height = 0;

	for (int i = 0; i < items.size(); i++) {
		child_start_y -= this->height;
		total_height += this->height;

		items[i].SetStartY(child_start_y);
	}

	createBox(this->width, total_height, this->start_pos_x, child_start_y);
}

void Dropdown::AddItems(std::vector<std::string> names) {
	items.clear();

	int child_start_y = this->start_pos_y;
	int total_height = 0;

	for (int i = 0; i < names.size(); i++) {
		child_start_y -= this->height;
		total_height += this->height;

		Button item = Button(window, width, height, start_pos_x, child_start_y, names[i]);
		item.Menu_id = i;
		item.SetOutline(glm::vec4(false, false, true, false));
		item.SetOnClick([this, i]() { this->setCurrent(i); });

		items.push_back(item);
	}
	createBox(this->width, total_height, this->start_pos_x, child_start_y);
	setCurrent(0);
}

void Dropdown::SetDropdown(bool visible) { this->dropdown_visible = visible; }

std::string Dropdown::GetSelected() { return this->currItem.GetText(); }
