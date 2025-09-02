#pragma once

#include <cstdio>
#include <glm/ext/vector_float3.hpp>

#include "../thirdparty/glad/glad.h"
#include "resource_manager.h"
#include <string>
#include <vector>

class Gui_Item {
	glm::vec3 BgColor = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 FgColor = glm::vec3(1.0f, 1.0f, 1.0f);

	GLuint VAO_Bg, VBO_Bg;

  protected:
	float height = 0;
	float width = 0;
	float start_pos_x;
	float start_pos_y;
	float window_width;
	float window_height;

	std::vector<glm::vec3> vertices;

	void drawSquare();
	void setupBuffer();

  public:
	enum class Type { BUTTON, TEXT_FIELD, MENU };

	Gui_Item(int width, int height, int start_pos_x, int start_pos_y) {
		json config = ResourceManager::Config;
		window_width = config["window"]["width"].get<int>() / 2.0f;
		window_height = config["window"]["height"].get<int>() / 2.0f;

		this->width = (width / window_width);
		this->height = (height / window_height);
		this->start_pos_x = (start_pos_x / window_width) - 1;
		this->start_pos_y = (start_pos_y / window_height) - 1;

		vertices.push_back({this->start_pos_x, this->start_pos_y, 0.0f});
		vertices.push_back({this->start_pos_x + this->width, this->start_pos_y, 0.0f});
		vertices.push_back(
			{this->start_pos_x + this->width, this->start_pos_y + this->height, 0.0f});

		vertices.push_back({this->start_pos_x, this->start_pos_y, 0.0f});
		vertices.push_back(
			{this->start_pos_x + this->width, this->start_pos_y + this->height, 0.0f});
		vertices.push_back({this->start_pos_x, this->start_pos_y + this->height, 0.0f});
		setupBuffer();
	}
	Gui_Item() {};

	void SetBgColor(float r, float g, float b);
	void SetFgColor(float r, float g, float b);

	float GetHeight();
	float GetWidth();

	virtual void Draw() = 0;
	virtual ~Gui_Item() = default;
};
