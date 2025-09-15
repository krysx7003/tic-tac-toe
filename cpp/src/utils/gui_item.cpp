#include "gui_item.h"

void Gui_Item::setupVertices() {
	float fWidth = (width / (window_width / 2.0f));
	float fHeight = (height / (window_height / 2.0f));
	float fStart_pos_x = (start_pos_x / (window_width / 2.0f)) - 1;
	float fStart_pos_y = (start_pos_y / (window_height / 2.0f)) - 1;

	glm::vec3 bottom_left = {fStart_pos_x, fStart_pos_y, 0.0f};
	glm::vec3 bottom_right = {fStart_pos_x + fWidth, fStart_pos_y, 0.0f};
	glm::vec3 top_left = {fStart_pos_x, fStart_pos_y + fHeight, 0.0f};
	glm::vec3 top_right = {fStart_pos_x + fWidth, fStart_pos_y + fHeight, 0.0f};

	background.Init(bottom_left, bottom_right, top_left, top_right);

	std::vector<glm::vec3> outline_vert;
	outline_vert.push_back(top_left);
	outline_vert.push_back(top_right);
	outline_vert.push_back(bottom_right);
	outline_vert.push_back(bottom_left);
	outline_vert.push_back(top_left);

	outline.Init(outline_vert);
}

glm::vec4 Gui_Item::hexToColor(std::string color) {
	if (color[0] != '#') {
		printf("ERROR::GUI_ITEM: Color is invalid\n");
		return glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	float r = std::stoi(color.substr(1, 2), nullptr, 16) / 255.0f;
	float g = std::stoi(color.substr(3, 2), nullptr, 16) / 255.0f;
	float b = std::stoi(color.substr(5, 2), nullptr, 16) / 255.0f;
	float a = 1.0f;
	if (color.size() == 9) {
		a = std::stoi(color.substr(7, 2), nullptr, 16) / 255.0f;
	}

	return glm::vec4(r, g, b, a);
}

void Gui_Item::SetBgColor(std::string color) { BgColor = hexToColor(color); }
void Gui_Item::SetBgColor(glm::vec4 color) { this->BgColor = color; }

void Gui_Item::SetFgColor(std::string color) {
	glm::vec4 rgba = hexToColor(color);
	FgColor = glm::vec3(rgba.r, rgba.g, rgba.b);
}
void Gui_Item::SetFgColor(glm::vec3 color) { this->FgColor = color; }

void Gui_Item::SetOutColor(std::string color) { OutColor = hexToColor(color); }
void Gui_Item::SetOutColor(glm::vec4 color) { this->OutColor = color; }

void Gui_Item::SetOutline(glm::bvec4 outline) { this->outlines = outline; }
void Gui_Item::SetOutlineWidth(float width) { outline.SetWidth(width); }

int Gui_Item::GetHeight() {
	if (this->Visible) {
		return height + padding_top;
	} else {
		return 0;
	}
}

int Gui_Item::GetWidth() { return width; }
int Gui_Item::GetPadding() { return padding_top; }

void Gui_Item::SetVisibility(bool visible) { this->Visible = visible; }

void Gui_Item::SetHeight(int new_height) {
	this->height = new_height;
	setupVertices();
}

void Gui_Item::SetStartY(int start_y) {
	this->start_pos_y = start_y;
	setupVertices();
}

void Gui_Item::SetPadding(int top) { this->padding_top = top; }
void Gui_Item::SetId(int id) { this->Menu_id = id; }

int Gui_Item::GetStartY() { return start_pos_y; }
