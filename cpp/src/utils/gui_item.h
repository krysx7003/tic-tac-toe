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
	int height = 0;
	int width = 0;
	int start_pos_x;
	int start_pos_y;
	float window_width;
	float window_height;

	std::vector<glm::vec3> vertices;

	void drawSquare();
	void setupBuffer();
	void setupVertices();

  public:
	enum class Type { BUTTON, TEXT_FIELD, MENU };

	Gui_Item(int width, int height, int start_pos_x, int start_pos_y)
		: width(width), height(height), start_pos_x(start_pos_x), start_pos_y(start_pos_y) {

		json config = ResourceManager::Config;
		window_width = config["window"]["width"].get<int>() / 2.0f;
		window_height = config["window"]["height"].get<int>() / 2.0f;

		setupVertices();
	}
	Gui_Item() {};

	void SetBgColor(float r, float g, float b);
	void SetFgColor(float r, float g, float b);
	void SetStartY(int start_y);

	int GetHeight();
	int GetStartY();
	int GetWidth();

	virtual void Draw() = 0;
	virtual ~Gui_Item() = default;
};
