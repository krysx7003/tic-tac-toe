#include "tile.h"

glm::vec4 Tile::hexToColor(std::string color) {
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

void Tile::setUpBackground() {
	float fWidth = (width / (window.x / 2.0f));
	float fHeight = (height / (window.y / 2.0f));
	float fStart_pos_x = (start_pos_x / (window.x / 2.0f)) - 1;
	float fStart_pos_y = (start_pos_y / (window.y / 2.0f)) - 1;

	glm::vec3 bottom_left = {fStart_pos_x, fStart_pos_y, 0.0f};
	glm::vec3 bottom_right = {fStart_pos_x + fWidth, fStart_pos_y, 0.0f};
	glm::vec3 top_left = {fStart_pos_x, fStart_pos_y + fHeight, 0.0f};
	glm::vec3 top_right = {fStart_pos_x + fWidth, fStart_pos_y + fHeight, 0.0f};

	background.Init(bottom_left, bottom_right, top_left, top_right);
}

void Tile::Render() {
	this->background.Draw(this->color);

	if (State == Tile::State::Empty) {
		return;
	}
	Texture2D texture;
	if (State == Tile::State::TakenO) {
		texture = ResourceManager::GetTexture("O");

	} else {
		texture = ResourceManager::GetTexture("X");
	}
	Renderer->DrawSprite(texture, glm::vec2(start_pos_x, start_pos_y), glm::vec2(width, height),
						 0.0f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

bool Tile::IsMouseOn(double mouseX, double mouseY) {
	if (start_pos_x > mouseX || mouseX > start_pos_x + width) {
		return false;
	}
	if (start_pos_y > mouseY || mouseY > start_pos_y + height) {
		return false;
	}

	return true;
}

void Tile::SetColor(std::string color_hex) { this->color = hexToColor(color_hex); }

void Tile::SetState(char state) { this->State = state; }
